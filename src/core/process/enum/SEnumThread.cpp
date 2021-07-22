#include "SEnumThread.h"
#include "global.h"
#include "SProcess.h"
#include "SThread.h"
#include "core\extras\NtExtras.h"

SEnumThread::SEnumThread(SProcess* process)
	: QThread()
	, _Process(process)
{
}

SEnumThread::~SEnumThread()
{
}

void SEnumThread::Stop()
{
	if (isInterruptionRequested()) {
		qWarning("Already stop.");
		return;
	}

	qDebug("Stopping...");
	requestInterruption();
	quit();
	wait();
	//if (_ExitSE.tryAcquire(1, 1000L))
	//	_ExitSE.release();
	//qInfo("Stopped");
}

void SEnumThread::run()
{
	THREADENTRY32 tlh32Entry;
	ZeroMemory(&tlh32Entry, sizeof(THREADENTRY32));
	tlh32Entry.dwSize = sizeof(THREADENTRY32);

	HANDLE hSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, _Process->GetID());
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		DWORD dwLastError = GetLastError();
		auto message = FormatLastError(dwLastError);
		qWarning("CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD) FAIL, 0x%08X %s",
			dwLastError,
			message.toUtf8().data());
		//_ExitSE.release();
		return;
	}

	while (!isInterruptionRequested())
	{
		quint64 nThreadCount = 0;
		BOOL bRet = Thread32First(hSnap, &tlh32Entry);
		do
		{
			if (tlh32Entry.th32OwnerProcessID == _Process->GetID())
			{
				if (!_Process->ThreadIsExist(tlh32Entry.th32ThreadID))
				{
					auto pThread = new SThread(tlh32Entry, _Process);
					pThread->start();
					_Process->AppendThread(pThread);
					nThreadCount++;
				}
			}
		} while (bRet = Thread32Next(hSnap, &tlh32Entry));

		if (nThreadCount > 0)
			qDebug("线程总数: %d", nThreadCount);
	}

	CloseHandle(hSnap);
	qDebug("Exit EnumThread");
}
