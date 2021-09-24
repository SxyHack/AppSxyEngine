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

QString FormatMemProtection(DWORD value)
{
	if (value == 0)
		return "PAGE_CALLER_NOT_ACCESS(0)";

	QString qsFormat;

	if (value & PAGE_EXECUTE)
		qsFormat += "PAGE_EXECUTE|";
	if (value & PAGE_EXECUTE_READ)
		qsFormat += "PAGE_EXECUTE_READ|";
	if (value & PAGE_EXECUTE_READWRITE)
		qsFormat += "PAGE_EXECUTE_READWRITE|";
	if (value & PAGE_EXECUTE_WRITECOPY)
		qsFormat += "PAGE_EXECUTE_WRITECOPY|";
	if (value & PAGE_NOACCESS)
		qsFormat += "PAGE_NOACCESS|";
	if (value & PAGE_READONLY)
		qsFormat += "PAGE_READONLY|";
	if (value & PAGE_READWRITE)
		qsFormat += "PAGE_READWRITE|";
	if (value & PAGE_WRITECOPY)
		qsFormat += "PAGE_WRITECOPY|";
	if (value & PAGE_TARGETS_INVALID)
		qsFormat += "PAGE_TARGETS_INVALID|";
	if (value & PAGE_GUARD)
		qsFormat += "PAGE_GUARD|";
	if (value & PAGE_NOCACHE)
		qsFormat += "PAGE_NOCACHE|";
	if (value & PAGE_WRITECOMBINE)
		qsFormat += "PAGE_WRITECOMBINE|";

	if (qsFormat.isEmpty())
		qsFormat = QString("PAGE_PROTECT_ERROR(0x%1)").arg(value, 0, 16);
	else
		qsFormat = qsFormat.left(qsFormat.length() - 1); // 删除最后一个'|'

	return qsFormat;
}

QString FormatMemState(DWORD value)
{
	QString qsFormat;
	if (value & MEM_COMMIT)
		qsFormat += "MEM_COMMIT|";
	if (value & MEM_FREE)
		qsFormat += "MEM_FREE|";
	if (value & MEM_RESERVE)
		qsFormat += "MEM_RESERVE|";

	if (qsFormat.isEmpty())
		qsFormat = QString("MEM_STATE_ERROR(0x%1)").arg(value, 0, 16);
	else
		qsFormat = qsFormat.left(qsFormat.length() - 1); // 删除最后一个'|'

	return qsFormat;
}

QString FormatMemType(DWORD value)
{
	QString qsFormat;

	if (value & MEM_IMAGE)
		qsFormat += "MEM_IMAGE|";
	if (value & MEM_MAPPED)
		qsFormat += "MEM_MAPPED|";
	if (value & MEM_PRIVATE)
		qsFormat += "MEM_PRIVATE|";

	if (qsFormat.isEmpty())
		qsFormat = QString("MEM_TYPE_ERROR(0x%1)").arg(value, 0, 16);
	else
		qsFormat = qsFormat.left(qsFormat.length() - 1); // 删除最后一个'|'

	return qsFormat;
}

QString GetSystemDir()
{
	TCHAR szPath[512] = { 0 };
	UINT nSize = GetSystemDirectory(szPath, 512);
	return QString::fromWCharArray(szPath, nSize);
}

