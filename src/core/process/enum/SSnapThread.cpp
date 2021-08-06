#include "SSnapThread.h"
#include "global.h"
#include "SProcess.h"
#include "SThread.h"
#include "SEnumThread.h"

#include "core\extras\NtExtras.h"

SSnapThread::SSnapThread(SProcess* process)
	: QThread()
	, _Process(process)
{
}

SSnapThread::~SSnapThread()
{
}

void SSnapThread::Stop()
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

void SSnapThread::run()
{
	//THREADENTRY32 tlh32Entry;
	//ZeroMemory(&tlh32Entry, sizeof(THREADENTRY32));
	//tlh32Entry.dwSize = sizeof(THREADENTRY32);

	while (!isInterruptionRequested())
	{
		//SElapsed elapse("拍摄线程快照");
		quint64 nThreadCount = 0;
		HANDLE hSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, _Process->GetID());
		if (hSnap == INVALID_HANDLE_VALUE)
		{
			DWORD dwLastError = GetLastError();
			auto message = FormatLastError(dwLastError);
			qWarning("CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD) FAIL, 0x%08X %s",
				dwLastError,
				message.toUtf8().data());
			return;
		}

		SEnumThread* pEnumThread = new SEnumThread(_Process, hSnap);
		pEnumThread->start(QThread::HighPriority);
		//pEnumThread->deleteLater();
		//BOOL bRet = Thread32First(hSnap, &tlh32Entry);
		//do
		//{
		//	if (tlh32Entry.th32OwnerProcessID == _Process->GetID())
		//	{
		//		if (!_Process->ThreadIsExist(tlh32Entry.th32ThreadID))
		//		{
		//			qDebug("发现新线程:[%d]", tlh32Entry.th32ThreadID);
		//			auto pThread = new SThread(tlh32Entry, _Process);
		//			_Process->AppendThread(pThread);
		//			pThread->start();
		//			nThreadCount++;
		//		}
		//	}
		//} while (bRet = Thread32Next(hSnap, &tlh32Entry));

		//if (nThreadCount > 0)
		//	qDebug("发现线程总数: %d", nThreadCount);

		//CloseHandle(hSnap);
	}

	qDebug("Exit EnumThread");

}
