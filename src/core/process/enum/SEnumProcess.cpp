#include "SEnumProcess.h"
#include "SProcess.h"
#include "SxyEngine.h"

SEnumProcess::SEnumProcess(const QString& filter)
	: QThread()
	, _Filter(filter)
{
}

SEnumProcess::~SEnumProcess()
{
	qDebug("~");
}

void SEnumProcess::run()
{
	BOOL ret = FALSE;
	DWORD dwRow = 0;
	PROCESSENTRY32 tlhEntry32 = { 0 };
	tlhEntry32.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		qWarning("CreateToolhelp32Snapshot FAIL, %0x08X", GetLastError());
		return;
	}

	ret = ::Process32First(hSnap, &tlhEntry32);
	do
	{
		// System process and Idle process have special PID.
		if (tlhEntry32.th32ProcessID == 0 || tlhEntry32.th32ProcessID == 4)
			continue;

		// ¹ýÂË×Ô¼º
		if (tlhEntry32.th32ProcessID == GetCurrentProcessId())
			continue;

		QString processName = QString::fromWCharArray(tlhEntry32.szExeFile);

		if (_Filter.isEmpty() || processName.contains(_Filter)) 
		{
			auto pProcess = new SProcess(tlhEntry32);
			SEngine.AppendProcess(pProcess);
			emit sgEnumProcess(dwRow++, pProcess);
		}
	} while (ret = ::Process32Next(hSnap, &tlhEntry32));

	CloseHandle(hSnap);

	emit sgEnumProcessDone();
}
