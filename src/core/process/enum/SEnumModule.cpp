#include "SEnumModule.h"
#include "SProcess.h"
#include "core\extras\NtExtras.h"

SEnumModule::SEnumModule(SProcess* process)
	: QThread(nullptr)
	, _Process(process)
{
}

SEnumModule::~SEnumModule()
{
	Stop();
}

void SEnumModule::Stop()
{
	if (!isRunning())
		return;

	if (isInterruptionRequested()) {
		qWarning("Already stop.");
		return;
	}

	qDebug("Stopping...");

	requestInterruption();
	if (_ExitSemaphore.tryAcquire(1, 1000L))
		_ExitSemaphore.release();

	qInfo("Stopped");
}

void SEnumModule::WaitForInit()
{
	_InitSemaphore.acquire();
}

void SEnumModule::run()
{
	MODULEENTRY32 tlh32Entry;
	ZeroMemory(&tlh32Entry, sizeof(MODULEENTRY32));
	tlh32Entry.dwSize = sizeof(MODULEENTRY32);

	HANDLE hSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, _Process->GetID());
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		DWORD dwLastError = GetLastError();
		auto message = FormatLastError(dwLastError);
		qWarning("CreateToolhelp32Snapshot(TH32CS_SNAPMODULE) FAIL, 0x%08X %s",
			dwLastError,
			message.toUtf8().data());
		_ExitSemaphore.release();
		return;
	}

	while (!isInterruptionRequested())
	{
		quint64 count = 0;
		BOOL ret = Module32First(hSnap, &tlh32Entry);
		do
		{
			if (tlh32Entry.th32ProcessID == _Process->GetID())
			{
				QString qsModName = QString::fromWCharArray(tlh32Entry.szModule);
				if (!_Process->ModuleIsExist(qsModName))
				{
					auto pModule = SModule::Create(_Process, tlh32Entry);
					_Process->AppendModule(pModule);
					if (pModule->Party == MODULE_PARTY::USER) {
						_Process->AppendModuleToWhitelist(pModule);
					}
					count++;
				}
			}
		} while ((ret = Module32Next(hSnap, &tlh32Entry)) && !isInterruptionRequested());

		if (count > 0) 
			qDebug("模块总数: %d", count);

		_InitSemaphore.release();
		QThread::msleep(500);
	}

	CloseHandle(hSnap);
	_ExitSemaphore.release();
	qDebug("Exit EnumModule");
}
