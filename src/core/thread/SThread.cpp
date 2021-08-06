#include "SThread.h"
#include "SProcess.h"
#include "core\extras\NtExtras.h"

//SThread::SThread(const THREADENTRY32& entry, SProcess* pProcess)
//	: QThread()
//	, _Process(pProcess)
//	, _Entry(entry)
//	, _Data(NULL)
//	, _Riper(this)
//	, _AddressOfOpcodeAccess(0)
//	, _Index(-1)
//{
//}

SThread::SThread(PSYSTEM_THREAD_INFORMATION pData, SProcess* pProcess)
	: QThread()
	, _Process(pProcess)
	, _Riper(this)
	, _AddressOfOpcodeAccess(0)
	, _Index(-1)
{
	CopyMemory(&_Data, pData, sizeof(_Data));
}

SThread::~SThread()
{
}

qint64 SThread::GetID()
{
	//return _Entry.th32ThreadID;
	return (qint64)_Data.ClientId.UniqueThread;
}

qint32 SThread::GetIndex()
{
	return _Index;
}

void SThread::SetIndex(qint32 nIndex)
{
	_Index = nIndex;
}

void SThread::FindOpcodeAccess(quint64 nAddress)
{
	_AddressOfOpcodeAccess = nAddress;
	_Riper.start();
}

void SThread::run()
{
	quint64 nLastRIP = 0;
	HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT, FALSE, GetID());
	if (hThread == NULL)
	{
		auto dwError = GetLastError();
		auto qsMessage = FormatLastError(dwError);
		qWarning("OpenThread Failed. %s(%d)", qsMessage.toUtf8().data(), dwError);
		_Process->RemoveThread(this);
		return;
	}
	qDebug("OpenThread[%d]", GetID());

	while (!isInterruptionRequested())
	{
		CONTEXT context;
		context.ContextFlags = CONTEXT_ALL | CONTEXT_FLOATING_POINT;
		if (!GetThreadContext(hThread, &context)) 
		{
			auto dwError = GetLastError();
			auto qsMessage = FormatLastError(dwError);
			qWarning("GetThreadContext(%d) Failed. %s(%d)", GetID(), qsMessage.toUtf8().data(), dwError);
			break;
		}

		if (nLastRIP != context.Rip)
		{
			nLastRIP = context.Rip;
			_ListRIP.append(context);
			if (_Index >= 9) {
				qDebug("线程[%d:%d] RIP:0x%X", GetID(), _Index, nLastRIP);
			}
		}

		//QThread::msleep(1);
	}

	CloseHandle(hThread);

	qDebug("跟踪线程[%d]结束.", GetID());
	_Process->RemoveThread(this);
}
