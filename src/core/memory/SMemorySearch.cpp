#include "SMemorySearch.h"
#include "SxyEngine.h"
#include "core\extras\NtExtras.h"
#include "settings\GlobalSettings.h"
#include "find\SFindWorker.h"

SMemorySearch::SMemorySearch(SProcess* pProcess)
	: QThread(nullptr)
	, _Process(pProcess)
	, _Method(nullptr)
	, _EnableCodeRegion(false)
	, _EnableMapped(false)
{
}

SMemorySearch::~SMemorySearch()
{
}

void SMemorySearch::FindWhatReset()
{
	_FindWhats.clear();
}

void SMemorySearch::FindWhat(const SFindWhat& what)
{
	_FindWhats.append(what);
}

void SMemorySearch::FindMethod(SFindMethod* pMethod)
{
	_Method = pMethod;
}

void SMemorySearch::EnableCodeRegion(bool enabled)
{
	_EnableCodeRegion = enabled;
}

void SMemorySearch::FindInRange(quint64 nStartAddr, quint64 nEndAddr)
{
	//SElapsed elapse(QString("FindInRange(%1)").arg(nEndAddr - nStartAddr, 8, 16, QLatin1Char('0')));

	const auto hProcess = _Process->GetHandle();
	TCHAR szModuleName[MAX_MODULE_SIZE] = { 0 };

	for (auto what : _FindWhats)
	{
		quint64 ulQueryAddr = nStartAddr;
		quint64 ulAllocBase = 0;
		MEMORY_BASIC_INFORMATION mbi;
		ZeroMemory(&mbi, sizeof(mbi));

		while (::VirtualQueryEx(hProcess, (LPVOID)ulQueryAddr, &mbi, sizeof(mbi)))
		{
			if (_Stop.tryAcquire()) {
				_Stop.release();
				return;
			}

			QByteArray qBytes;
			QString qsModuleName;
			quint32 nRetryCount = 1;
			quint32 nErrorNum = 0; 
			quint64 nBegRegionAddr = 0;
			quint64 nEndRegionAddr = (quint64)mbi.BaseAddress + mbi.RegionSize;
			SModule* pModule = _Process->GetModuleName(quint64(mbi.BaseAddress), qsModuleName); // 获取模块名字
			auto qsAllocMemProtect = FormatMemProtection(mbi.AllocationProtect);
			auto qsMemProtect = FormatMemProtection(mbi.Protect);
			auto qsMemState = FormatMemState(mbi.State);
			auto qsMemType = FormatMemType(mbi.Type);
			bool bMapped = (mbi.Type == MEM_MAPPED);
			bool bReserved = (mbi.State == MEM_RESERVE);

			if (mbi.State == MEM_FREE)
				goto LOOP_END;

			// 如果是MEM_MAPPED，则根据配置是否跳过
			if (bMapped && !_EnableMapped) 
				goto LOOP_END;

			// 如果没有获取到模块，再获取一次
			if (pModule == nullptr)
			{
				::GetModuleBaseName(hProcess, (HMODULE)mbi.AllocationBase, szModuleName, MAX_MODULE_SIZE);
				qsModuleName = QString::fromWCharArray(szModuleName);
				//qWarning("%p 第2次获取模块名", mbi.BaseAddress);
			}

			// 如果没有获取到模块名，再获取一次
			if (qsModuleName.isEmpty()) 
			{
				TCHAR szMappedName[MAX_MODULE_SIZE] = L"";
				if (bMapped && (GetMappedFileName(hProcess, mbi.AllocationBase, szMappedName, MAX_MODULE_SIZE) != 0))
				{
					qsModuleName = QString::fromWCharArray(szMappedName);
				}
				//qWarning("%p 第3次获取模块名", mbi.BaseAddress);
			}

			// 如果模块不在白名单，不搜索该模块
			if (!qsModuleName.isEmpty() && !_Process->InWhitelist(qsModuleName))
			{
				//qCritical("跳过, 模块[%s-%p]不在白名单", qsModuleName.toUtf8().data(), mbi.BaseAddress);
				goto LOOP_END;
			}

			// 如果是代码页，根据配置是否跳过该模块
			if (_Process->IsCodeRegion(mbi.BaseAddress) && !_EnableCodeRegion)
			{
				//qCritical("跳过, 区域[%p]属于代码页", mbi.BaseAddress);
				goto LOOP_END;
			}

			// 读取首部(What.Size)个字节，判断这个内存页是否可以读取, 如果失败重试一次
			while (!_Process->ReadMemory(qBytes, mbi.BaseAddress, what.Size))
			{
				if (0 >= nRetryCount--) { 
					goto LOOP_END;
				}
			}

			//qDebug("[%s] RegionInfo: [%p<%p<%p] %p %s %s %s",
			//	qsModuleName.isEmpty() ? "PRIVATE" : qsModuleName.toUtf8().data(),
			//	nStartAddr, mbi.BaseAddress, nEndAddr,
			//	mbi.AllocationBase,
			//	qsMemProtect.toUtf8().data(),
			//	qsMemState.toUtf8().data(),
			//	qsMemType.toUtf8().data());

			if (_Method->Match(qBytes, what))
			{
				what.AppendBuff(SMemoryBuffer((quint64)mbi.BaseAddress, qBytes, pModule));
			}
			
			nBegRegionAddr = (quint64)mbi.BaseAddress + what.Size;

			// 搜索剩下的内存
			while (nBegRegionAddr < nEndRegionAddr)
			{
				QByteArray qReadBytes;
				if (!_Process->ReadMemory(qReadBytes, (LPVOID)nBegRegionAddr, what.Size))
				{
					nBegRegionAddr += what.Size;
					if (++nErrorNum > 0xF)// 读取内存连续出错0xF次，则判定该内存页出现问题
					{
						qDebug("读取内存值连续失败(%d)次", nErrorNum-1);
						break;
					}

					continue;
				}

				if (_Method->Match(qReadBytes, what))
				{
					what.AppendBuff(SMemoryBuffer(nBegRegionAddr, qReadBytes, pModule));
				}

				nBegRegionAddr += what.Size;
				nErrorNum = 0;
			}

		LOOP_END:
			quint64 ulNextRegionAddr = (quint64)mbi.BaseAddress + mbi.RegionSize;
			if (ulNextRegionAddr <= ulQueryAddr || ulNextRegionAddr >= nEndAddr)
				break;

			ulQueryAddr = ulNextRegionAddr;
		}
		//qDebug("找到地址: %d", what.GetFoundCount());
	}
}

void SMemorySearch::Cancel()
{
	_Stop.release();
}

void SMemorySearch::run()
{
	SElapsed elapse("搜索内存");
	SYSTEM_INFO si;
	ZeroMemory(&si, sizeof(si));
	GetSystemInfo(&si);

	DWORD nNumberOfProcessors = qMin(si.dwNumberOfProcessors, (DWORD)10);
	_ThreadPool.setMaxThreadCount(nNumberOfProcessors);

	quint64 nAppBegAddr = (quint64)si.lpMinimumApplicationAddress;
	quint64 nAppEndAddr = (quint64)si.lpMaximumApplicationAddress;
	quint64 nAppSize = nAppEndAddr - nAppBegAddr;
	quint64 nWorkSize = nAppSize / 2048;
	//quint64 nWorkSize = 4 * 1024 * 1024;

	qDebug("程序虚拟地址范围: [%p, %p]", nAppBegAddr, nAppEndAddr);
	quint64 nWorkBegAddress = nAppBegAddr;
	quint64 nWorkEndAddress = nWorkBegAddress + nWorkSize;

	while (nWorkBegAddress < nAppEndAddr)
	//for (auto i = 0; i < nNumberOfProcessors; i++)
	{
		if (nWorkEndAddress > nAppEndAddr)
			nWorkEndAddress = nAppEndAddr;

		//qDebug("建立并行处理: [%p, %p]", nWorkBegAddress, nWorkEndAddress);

		SFindWorker* pWorker = new SFindWorker(this, nWorkBegAddress, nWorkEndAddress);
		_ThreadPool.start(pWorker);
		nWorkBegAddress += nWorkSize;
		nWorkEndAddress = nWorkBegAddress + nWorkSize;
	}

	_ThreadPool.waitForDone();
}
