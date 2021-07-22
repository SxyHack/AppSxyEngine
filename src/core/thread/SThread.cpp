#include "SThread.h"
#include "SProcess.h"
#include "core\extras\NtExtras.h"

SThread::SThread(const THREADENTRY32& entry, SProcess* pProcess)
	: QThread()
	, _Process(pProcess)
	, _Entry(entry)
	, _LastRIP(0)
{
}

SThread::~SThread()
{
}

qint32 SThread::GetID()
{
	return _Entry.th32ThreadID;
}

void SThread::run()
{
	HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT, FALSE, GetID());
	if (hThread == NULL)
	{
		auto dwError = GetLastError();
		auto qsMessage = FormatLastError(dwError);
		qWarning("OpenThread Failed. %s(%d)", qsMessage.toUtf8().data(), dwError);
		_Process->RemoveThread(this);
		return;
	}

	while (!isInterruptionRequested())
	{
		CONTEXT context;
		context.ContextFlags = CONTEXT_ALL | CONTEXT_FLOATING_POINT;
		if (!GetThreadContext(hThread, &context)) 
		{
			//ResumeThread(hThread);
			continue;
		}

		if (_LastRIP != context.Rip) 
		{
			_LastRIP = context.Rip;
			_ListRIP.append(context);
			qDebug("线程[%d] RIP:%X", GetID(), _LastRIP);
		}

		QThread::msleep(1);
	}

	CloseHandle(hThread);
	qDebug("跟踪线程[%d]结束.", GetID());
}
