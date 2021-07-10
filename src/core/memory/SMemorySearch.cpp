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
	, _Done(false)
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

void SMemorySearch::EnableMapped(bool enabled)
{
	_EnableMapped = enabled;
}

void SMemorySearch::FindInRange(quint64 nBegAddr, quint64 nEndAddr, SFindWhat& what, SModule* pModule)
{
	//SElapsed elapse(QString("FindInRange(%1)").arg(nEndAddr - nStartAddr, 8, 16, QLatin1Char('0')));
	while (nBegAddr < nEndAddr)
	{
		QByteArray qByteArray;
		if (!_Process->ReadMemory(qByteArray, (LPVOID)nBegAddr, what.Size))
		{
			nBegAddr += what.Size;
			continue;
		}

		if (_Method->Match(qByteArray, what))
		{
			what.AppendBuff(SMemoryBuffer(nBegAddr, qByteArray, pModule));
		}

		nBegAddr += what.Size;
	}
}

void SMemorySearch::Cancel()
{
	_Stop.release();
}

bool SMemorySearch::IsDone()
{
	return _Done;
}

void SMemorySearch::run()
{
	SElapsed elapse("搜索内存");

	SYSTEM_INFO si;
	ZeroMemory(&si, sizeof(si));
	GetSystemInfo(&si);

	TCHAR szModuleName[MAX_MODULE_SIZE] = { 0 };
	auto hProcess = _Process->GetHandle();
	DWORD nNumberOfProcessors = qMax(si.dwNumberOfProcessors, (DWORD)10);
	_ThreadPool.setMaxThreadCount(nNumberOfProcessors);

	quint64 nAppBegAddr = (quint64)si.lpMinimumApplicationAddress;
	quint64 nAppEndAddr = (quint64)si.lpMaximumApplicationAddress;
	quint64 nAppSize = nAppEndAddr - nAppBegAddr;

	for (auto what : _FindWhats)
	{
		quint64 ulQueryAddr = nAppBegAddr;
		MEMORY_BASIC_INFORMATION mbi;
		ZeroMemory(&mbi, sizeof(mbi));

		while (::VirtualQueryEx(hProcess, (LPVOID)ulQueryAddr, &mbi, sizeof(mbi)))
		{
			if (_Stop.tryAcquire()) {
				_Stop.release();
				return;
			}

			QByteArray qByteArray;
			QString qModuleName;
			quint32 nRetryCount = 1;
			quint32 nErrorNum = 0;
			quint64 nBegRegionAddr = (quint64)mbi.BaseAddress;
			quint64 nEndRegionAddr = (quint64)mbi.BaseAddress + mbi.RegionSize - 1;
			// 每个工作线程处理的长度
			quint64 nWorkSize = mbi.RegionSize / nNumberOfProcessors;
			SModule* pModule = _Process->GetModuleName(nBegRegionAddr, qModuleName); // 获取模块名字
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
				qModuleName = QString::fromWCharArray(szModuleName);
			}

			// 如果没有获取到模块名，再获取一次
			if (qModuleName.isEmpty())
			{
				TCHAR szMappedName[MAX_MODULE_SIZE] = L"";
				if (bMapped && (GetMappedFileName(hProcess, mbi.AllocationBase, szMappedName, MAX_MODULE_SIZE) != 0))
				{
					qModuleName = QString::fromWCharArray(szMappedName);
				}
			}

			// 如果模块不在白名单，不搜索该模块
			if (!qModuleName.isEmpty() && !_Process->InWhitelist(qModuleName)) {
				//qCritical("跳过, 模块[%s-%p]不在白名单", qsModuleName.toUtf8().data(), mbi.BaseAddress);
				goto LOOP_END;
			}

			// 如果是代码页，根据配置是否跳过该模块
			if (_Process->IsCodeRegion(mbi.BaseAddress) && !_EnableCodeRegion) {
				//qCritical("跳过, 区域[%p]属于代码页", mbi.BaseAddress);
				goto LOOP_END;
			}

			// 读取首部(What.Size)个字节，判断这个内存页是否可以读取, 如果失败重试一次
			while (!_Process->ReadMemory(qByteArray, mbi.BaseAddress, what.Size)) {
				if (0 >= nRetryCount--) {
					goto LOOP_END;
				}
			}

			if (_Method->Match(qByteArray, what)) {
				what.AppendBuff(SMemoryBuffer(nBegRegionAddr, qByteArray, pModule));
			}

			nWorkSize += nWorkSize % what.Size;

			quint64 nBegWorkAddr = nBegRegionAddr + what.Size;
			quint64 nEndWorkAddr = nBegWorkAddr + nWorkSize - 1;

			while (nBegWorkAddr < nEndRegionAddr)
			{
				_ThreadPool.start(new SFindWorker(this, 
					pModule, 
					what, 
					nBegWorkAddr, 
					qMin(nEndWorkAddr, nEndRegionAddr)));
				nBegWorkAddr += nWorkSize;
				nEndWorkAddr += nWorkSize;
			}

		LOOP_END:
			quint64 ulNextRegionAddr = nBegRegionAddr + mbi.RegionSize;
			if (ulNextRegionAddr <= ulQueryAddr || ulNextRegionAddr >= nAppEndAddr)
				break;
			ulQueryAddr = ulNextRegionAddr;
		}
	}

	_ThreadPool.waitForDone();
}
