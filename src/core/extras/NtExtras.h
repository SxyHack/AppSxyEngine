#pragma once
#include <QString>
#include "global.h"

QString FormatLastError(DWORD lastErr);
DWORD   NtErrToWinError(NTSTATUS status);

QString FormatMemProtection(DWORD value);
QString FormatMemState(DWORD value);
QString FormatMemType(DWORD value);

QString GetSystemDir();