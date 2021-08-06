#include "SEnumThreadNT.h"
#include "SProcess.h"

#include "libs\ntdll\ntdll.h"
#include "core\extras\NtExtras.h"

SEnumThreadNT::SEnumThreadNT(SProcess* process)
	: QThread()
	, _Process(process)
{
}

SEnumThreadNT::~SEnumThreadNT()
{
}

void SEnumThreadNT::Stop()
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

//typedef NTSTATUS(__stdcall* NTQUERYSYSTEMINFORMATION)
//(IN      SYSTEM_INFORMATION_CLASS SystemInformationClass,
//	IN OUT  PVOID                    SystemInformation,
//	IN      ULONG                    SystemInformationLength,
//	OUT     PULONG                   ReturnLength  OPTIONAL);
//NTQUERYSYSTEMINFORMATION NtQuerySystemInformation2;

void SEnumThreadNT::run()
{
	SElapsed elapse("NtEnumThread");
	ULONG nReturnLength = 0;
	ULONG nBufferLength = 1;
	PSYSTEM_PROCESS_INFORMATION pBuffer = (PSYSTEM_PROCESS_INFORMATION)malloc(nBufferLength);
	if (pBuffer == NULL)
	{
		qDebug("无法获取进程信息，内存不足");
		return;
	}

	ZeroMemory(pBuffer, nBufferLength);

	if (NtQuerySystemInformation(SystemProcessInformation, pBuffer, nBufferLength, &nReturnLength) == STATUS_INFO_LENGTH_MISMATCH)
	{
		free(pBuffer);
		nBufferLength = nReturnLength + sizeof(SYSTEM_PROCESS_INFORMATION);
		pBuffer = (PSYSTEM_PROCESS_INFORMATION)malloc(nBufferLength);
		if (!pBuffer)
			return;

		if (NtQuerySystemInformation(SystemProcessInformation, pBuffer, nBufferLength, &nReturnLength) != STATUS_SUCCESS)
		{
			qWarning("NtQuerySystemInformation Failed.");
			free(pBuffer);
			return;
		}
	}

	PSYSTEM_PROCESS_INFORMATION pspi = pBuffer;
	while (pspi->NextEntryOffset != 0)
	{
		if (pspi->UniqueProcessId == (HANDLE)_Process->GetID())
		{
			PSYSTEM_THREAD_INFORMATION pThreadData = &pspi->Threads[0];
			for (ULONG i = 0; i < pspi->NumberOfThreads; i++)
			{
				auto nThreadID = (qint64)pThreadData->ClientId.UniqueThread;
				if (!_Process->ThreadIsExist(nThreadID))
				{
					SThread* pThread = new SThread(pThreadData, _Process);
					_Process->AppendThread(pThread);
					pThread->start();
				}
				qDebug("ThreadID: %d", nThreadID);
				pThreadData++;
			}

			qDebug("==============================================================\n");
			break;
		}
		pspi = (PSYSTEM_PROCESS_INFORMATION)((DWORD_PTR)pspi + (DWORD_PTR)pspi->NextEntryOffset);
	}

	free(pBuffer);
}
