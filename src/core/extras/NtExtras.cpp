#include "NtExtras.h"
#include "libs/ntdll/ntdll.h"

QString FormatLastError(DWORD lastErr)
{
	LPTSTR lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		lastErr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

#ifdef UNICODE
	return QString::fromWCharArray(lpMsgBuf);
#else
	return QString::fromLocal8Bit(lpMsgBuf);
#endif

}

DWORD NtErrToWinError(NTSTATUS status)
{
	DWORD dwErrCode = RtlNtStatusToDosError(status);
	SetLastError(dwErrCode);
	return dwErrCode;
}

QString GetSystemDir()
{
	TCHAR szPath[512] = { 0 };
	UINT nSize = GetSystemDirectory(szPath, 512);
	return QString::fromWCharArray(szPath, nSize);
}

