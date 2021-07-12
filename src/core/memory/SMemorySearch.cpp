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
	, _ProcessTotalBytes(0)
	, _ProcessReadedBytes(0)
{
	SYSTEM_INFO si;
	ZeroMemory(&si, sizeof(si));
	GetSystemInfo(&si);

	_NumberOfProcessors = qMax(si.dwNumberOfProcessors, (DWORD)10);
	_ThreadPool.setMaxThreadCount(_NumberOfProcessors);

	_ProcessBegAddress = (quint64)si.lpMinimumApplicationAddress;
	_ProcessEndAddress = (quint64)si.lpMaximumApplicationAddress;
	_ProcessTotalBytes = _ProcessEndAddress - _ProcessBegAddress;
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
		if (_Process->ReadMemory(qByteArray, (LPVOID)nBegAddr, what.Size))
		{
			if (_Method && _Method->Match(qByteArray, what))
			{
				what.AppendBuff(SMemoryBuffer(nBegAddr, qByteArray, pModule));
			}
		}

		nBegAddr += what.Size;
		_ProcessReadedBytes += what.Size;
	}
}

void SMemorySearch::Cancel()
{
	_Stop.release();
}

void SMemorySearch::Reset()
{
	_Done = false;
	_ProcessTotalBytes = 0;
	_ProcessReadedBytes = 0;
}

bool SMemorySearch::IsDone()
{
	return _Done;
}

bool SMemorySearch::IsValidRegion(const MEMORY_BASIC_INFORMATION& mbi)
{
	if (mbi.State == MEM_FREE)
		return false;

	// 如果是MEM_MAPPED，则根据配置是否跳过
	if (mbi.Type == MEM_MAPPED && !_EnableMapped)
		return false;

	if (mbi.Protect == 0)
		return false;

	// 如果是写时拷贝，跳过
	if ((mbi.Protect & PAGE_WRITECOPY))
		return false;

	if (mbi.Protect & PAGE_NOCACHE)
		return false;
	if (mbi.Protect & PAGE_NOACCESS)
		return false;
	if (mbi.Protect & PAGE_GUARD)
		return false;

	return true;
}

SWHAT_LIST& SMemorySearch::GetWhatList()
{
	return _FindWhats;
}

quint32 SMemorySearch::GetWhatCount()
{
	quint32 nCount = 0;
	for (auto what : _FindWhats) {
		nCount += what.GetFoundCount();
	}

	return nCount;
}

quint64 SMemorySearch::GetReadedBytes()
{
	return _ProcessReadedBytes;
}

quint64 SMemorySearch::GetTotalBytes()
{
	return _ProcessTotalBytes;
}

void SMemorySearch::run()
{
	SElapsed elapse("搜索内存");

	TCHAR szModuleName[MAX_MODULE_SIZE] = { 0 };
	auto hProcess = _Process->GetHandle();

	for (auto what : _FindWhats)
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
			quint64 nWorkSize = mbi.RegionSize / _NumberOfProcessors;
			SModule* pModule = _Process->GetModuleName(nBegRegionAddr, qModuleName); // 获取模块名字
			auto qsAllocMemProtect = FormatMemProtection(mbi.AllocationProtect);
			auto qsMemProtect = FormatMemProtection(mbi.Protect);
			auto qsMemState = FormatMemState(mbi.State);
			auto qsMemType = FormatMemType(mbi.Type);
			bool bMapped = (mbi.Type == MEM_MAPPED);
			bool bReserved = (mbi.State == MEM_RESERVE);

			if (IsValidRegion(mbi)) {
				_ProcessReadedBytes += mbi.RegionSize;
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
				_ProcessReadedBytes += mbi.RegionSize;
				goto LOOP_END;
			}

			// 如果是代码页，根据配置是否跳过该模块
			if (_Process->IsCodeRegion(mbi) && !_EnableCodeRegion) {
				_ProcessReadedBytes += mbi.RegionSize;
				goto LOOP_END;
			}

			// 读取首部(What.Size)个字节，判断这个内存页是否可以读取, 如果失败重试一次
			while (!_Process->ReadMemory(qByteArray, mbi.BaseAddress, what.Size)) {
				if (0 >= nRetryCount--) {
					_ProcessReadedBytes += mbi.RegionSize;
					goto LOOP_END;
				}
			}

			_ProcessReadedBytes += what.Size;

			if (_Method && _Method->Match(qByteArray, what)) {
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
			quint64 nNextRegionAddr = nBegRegionAddr + mbi.RegionSize;
			if (nNextRegionAddr <= ulQueryAddr || nNextRegionAddr >= _ProcessEndAddress)
				break;
			ulQueryAddr = nNextRegionAddr;
		}
	}

	_ThreadPool.waitForDone();
	_Done = true;

	emit sgSearchDone(GetWhatCount());
}
