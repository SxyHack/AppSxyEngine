#include "SMemorySearch.h"
#include "SxyEngine.h"
#include "SProcess.h"

#include "core\extras\NtExtras.h"
#include "settings\GlobalSettings.h"
#include "find\SFindWorker.h"

SMemorySearch::SMemorySearch(SProcess* pProcess)
	: SMemoryAction(pProcess)
	, _EnableCodeRegion(false)
	, _EnableMapped(false)
{
	_ThreadPool.setMaxThreadCount(_NumberOfProcessors);

	//connect(this, SIGNAL(sgSearchDone(quint32)), pProcess, SIGNAL(sgSearchDone(quint32)));
}

SMemorySearch::~SMemorySearch()
{
	delete _Method;
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
	//SElapsed elapse(QString("FindInRange(%1, %2)")
	//	.arg(nBegAddr, 16, 16)
	//	.arg(nEndAddr, 16, 16));
	const int BUFF_SIZE = 0x2000;
	auto pBuffer = new char[BUFF_SIZE];
	auto hProcess = _Process->GetHandle();
	auto nReadedSize = (SIZE_T)0;
	ZeroMemory(pBuffer, BUFF_SIZE);

	while (nBegAddr < nEndAddr)
	{
		if (ReadProcessMemory(hProcess, (LPVOID)nBegAddr, pBuffer, BUFF_SIZE, &nReadedSize))
		{
			for (int i = 0; i < nReadedSize; i += what.Size)
			{
				char* pOffset = pBuffer + i;
				auto nOffsetAddr = nBegAddr + i;

				if (0x7FF77BEF6AC0 <= nOffsetAddr && nOffsetAddr <= 0x7FF77BEF6AD8)
				{
					qDebug("Address:%p, %x", nOffsetAddr, nReadedSize);
				}

				if (_Method && _Method->Match(pOffset, what))
				{
					what.AppendBuff(SMemoryBuffer(nOffsetAddr, pOffset, &what, pModule, _Process));
				}
			}
		}
		else
		{
			break;
		}

		//QByteArray qBuffer;
		//if (_Process->ReadMemory(qBuffer, (LPVOID)nBegAddr, BUFF_SIZE))
		//{
		//	//auto nPos = 0;
		//	//auto nCount = qBuffer.count() / what.Size;
		//	for (int i = 0; i < qBuffer.count(); i+= what.Size)
		//	{
		//		auto byteN = qBuffer.mid(i, what.Size);
		//		if (_Method && _Method->Match(qBuffer, what))
		//		{
		//			what.AppendBuff(SMemoryBuffer(nBegAddr, qBuffer, pModule));
		//		}
		//	}
		//}

		nBegAddr += BUFF_SIZE;
		_Process->NumberOfSearch += BUFF_SIZE;
	}

	delete[] pBuffer;
}

void SMemorySearch::Reset()
{

}

bool SMemorySearch::IsValidRegion(const MEMORY_BASIC_INFORMATION& mbi)
{
	if (mbi.State == MEM_FREE)
		return false;

	// 如果是写时拷贝，跳过
	if ((mbi.Protect & PAGE_WRITECOPY) ||
		(mbi.Protect & PAGE_NOCACHE) ||
		(mbi.Protect & PAGE_NOACCESS) ||
		(mbi.Protect & PAGE_GUARD) ||
		(mbi.Protect == 0))
		return false;

	// 如果是MEM_MAPPED，则根据配置是否跳过
	if (mbi.Type & MEM_MAPPED || mbi.Type == MEM_MAPPED)
		return _EnableMapped;

	return true;
}

void SMemorySearch::run()
{
	SElapsed elapse("搜索内存");

	TCHAR szModuleName[MAX_MODULE_SIZE] = { 0 };
	auto hProcess = _Process->GetHandle();

	for (auto& what : _FindWhats)
	{
		quint64 ulQueryAddr = _ProcessBegAddress;
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
			quint64 nWorkSize = 0x8000; // mbi.RegionSize / 0x200; // nWorkSize += nWorkSize % what.Size;

			SModule* pModule = _Process->GetModuleName(nBegRegionAddr, qModuleName); // 获取模块名字
			auto qsAllocMemProtect = FormatMemProtection(mbi.AllocationProtect);
			auto qsMemProtect = FormatMemProtection(mbi.Protect);
			auto qsMemState = FormatMemState(mbi.State);
			auto qsMemType = FormatMemType(mbi.Type);
			bool bMapped = (mbi.Type == MEM_MAPPED);
			bool bReserved = (mbi.State == MEM_RESERVE);

			if (!IsValidRegion(mbi)) {
				_Process->NumberOfSearch += mbi.RegionSize;
				goto LOOP_END;
			}

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
				_Process->NumberOfSearch += mbi.RegionSize;
				goto LOOP_END;
			}

			qDebug("搜索内存页:%p:[%s %s %s] (%s)",
				mbi.BaseAddress,
				qsMemProtect.toUtf8().data(),
				qsMemState.toUtf8().data(),
				qsMemType.toUtf8().data(),
				qModuleName.toUtf8().data());

			// 如果是代码页，根据配置是否跳过该模块
			if (_Process->IsCodeRegion(mbi) && !_EnableCodeRegion) {
				_Process->NumberOfSearch += mbi.RegionSize;
				goto LOOP_END;
			}

			quint64 nBegWorkAddr = nBegRegionAddr;
			while (nBegWorkAddr < nEndRegionAddr)
			{
				quint64 nFixRegionSize = qMin(nWorkSize, mbi.RegionSize);
				quint64 nEndWorkAddr = nBegWorkAddr + nFixRegionSize;

				//qDebug("建立并行处理: [%p, %p](%X)", nBegWorkAddr, nEndWorkAddr, nFixRegionSize);
				_ThreadPool.start(new SFindWorker(this,
					pModule,
					&what,
					nBegWorkAddr,
					qMin(nEndWorkAddr, nEndRegionAddr)));
				nBegWorkAddr += nFixRegionSize;
			}

		LOOP_END:
			quint64 nNextRegionAddr = nBegRegionAddr + mbi.RegionSize;
			if (nNextRegionAddr <= ulQueryAddr || nNextRegionAddr >= _ProcessEndAddress)
				break;
			ulQueryAddr = nNextRegionAddr;
		}
	}

	_ThreadPool.waitForDone();
	_Process->PushMemoryAction(this);
	auto nFoundCount = GetFoundCount();
	emit _Process->sgSearchDone(this, nFoundCount);
}
