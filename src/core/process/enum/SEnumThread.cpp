#include "SEnumThread.h"
#include "SProcess.h"
#include "SThread.h"
#include "core\extras\NtExtras.h"

SEnumThread::SEnumThread(SProcess* process, HANDLE hSnap)
	: QThread()
	, _Process(process)
	, _hSnap(hSnap)
{
}

SEnumThread::~SEnumThread()
{
	qDebug("~");
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
}

void SEnumThread::run()
{
	//SElapsed elapse("拍摄线程快照");

	quint64 nThreadCount = 0;
	THREADENTRY32 tlh32Entry;
	ZeroMemory(&tlh32Entry, sizeof(THREADENTRY32));
	tlh32Entry.dwSize = sizeof(THREADENTRY32);

	BOOL bRet = Thread32First(_hSnap, &tlh32Entry);
	do
	{
		if (tlh32Entry.th32OwnerProcessID == _Process->GetID())
		{
			if (!_Process->ThreadIsExist(tlh32Entry.th32ThreadID))
			{
				qDebug("发现新线程:[%d]", tlh32Entry.th32ThreadID);
				//auto pThread = new SThread(tlh32Entry, _Process);
				//_Process->AppendThread(pThread);
				//pThread->start();
				nThreadCount++;
			}
		}
	} while (bRet = Thread32Next(_hSnap, &tlh32Entry));

	if (nThreadCount > 0)
		qDebug("发现线程总数: %d", nThreadCount);

	CloseHandle(_hSnap);

	qDebug("Exit EnumThread");

	//deleteLater();
}
