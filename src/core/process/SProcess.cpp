#include "SProcess.h"
#include "core\extras\NtExtras.h"
#include "libs\ntdll\ntdll.h"

SProcess::SProcess(QObject *parent)
	: QObject(parent)
{
}

SProcess::~SProcess()
{
}

BOOL SProcess::Open(DWORD dwPID)
{
	_ID = dwPID;
	_Handle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, _ID);
	if (_Handle == NULL)
	{
		_Error = GetLastError();
		_ErrMessage = FormatLastError(_Error);
		qCritical("OpenProcess[%d] failed, ERROR:0x%x", _ID, _Error);
		return FALSE;
	}

	return TRUE;
}

BOOL SProcess::NtOpen(DWORD dwPID)
{
	NTSTATUS status;
	OBJECT_ATTRIBUTES objectAttributes;
	CLIENT_ID clientId;

	_ID = dwPID;
	/* If we don't have a PID, look it up */
	//if (dwProcessId == MAXDWORD) dwProcessId = (DWORD_PTR)CsrGetProcessId();

	/* Open a handle to the process */
	clientId.UniqueThread = NULL;
	clientId.UniqueProcess = UlongToHandle(dwPID);
	InitializeObjectAttributes(&objectAttributes, NULL, 0, NULL, NULL);
	status = NtOpenProcess(&_Handle,
		PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION |
		PROCESS_VM_WRITE | PROCESS_VM_READ |
		PROCESS_SUSPEND_RESUME | PROCESS_QUERY_INFORMATION,
		&objectAttributes,
		&clientId);
	if (!NT_SUCCESS(status))
	{
		/* Fail */
		NtErrToWinError(status);
		return FALSE;
	}

	/* Return the handle */
	return TRUE;
}

BOOL SProcess::IsOpen()
{
	return _Handle != NULL;
}

BOOL SProcess::Close()
{
	NtClose(_Handle);
}

BOOL SProcess::IsWow64()
{
	BOOL bIsWow64 = FALSE;
	if (!IsWow64Process(_Handle, &bIsWow64))
		return FALSE;

	return bIsWow64;
}

quint64 SProcess::GetID()
{
	return _ID;
}

void SProcess::AppendModule(SModule* pModule)
{

}

bool SProcess::ModuleIsExist(const QString& name)
{

}

quint64 SProcess::GetModuleCount()
{
	return 0;
}

SModule* SProcess::GetModule()
{
	return NULL;
}

SProcess::operator HANDLE()
{
	return _Handle;
}
