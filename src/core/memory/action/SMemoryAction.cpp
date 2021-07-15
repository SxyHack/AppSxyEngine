#include "SMemoryAction.h"
#include "SProcess.h"

SMemoryAction::SMemoryAction(SProcess* pProcess)
	: QThread()
	, _Method(nullptr)
	, _Process(pProcess)
{
	SYSTEM_INFO si;
	ZeroMemory(&si, sizeof(si));
	GetSystemInfo(&si);
	_NumberOfProcessors = qMin(si.dwNumberOfProcessors, (DWORD)10);

	_ProcessBegAddress = (quint64)si.lpMinimumApplicationAddress;
	_ProcessEndAddress = (quint64)si.lpMaximumApplicationAddress;
	_Process->NumberOfVirtualMemory = _ProcessEndAddress - _ProcessBegAddress;
}

SMemoryAction::~SMemoryAction()
{
}

void SMemoryAction::Cancel()
{
	_Stop.release();
}

void SMemoryAction::FindWhatReset()
{
	_FindWhats.clear();
}

void SMemoryAction::FindWhat(const SFindWhat& what)
{
	_FindWhats.append(what);
}

void SMemoryAction::FindMethod(SFindMethod* pMethod)
{
	_Method = pMethod;
}

SWHAT_LIST& SMemoryAction::GetWhatList()
{
	return _FindWhats;
}

quint32 SMemoryAction::GetFoundCount()
{
	quint32 nCount = 0;
	for (auto& what : _FindWhats) {
		nCount += what.GetFoundCount();
	}

	return nCount;
}
