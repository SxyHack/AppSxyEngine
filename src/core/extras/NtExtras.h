#pragma once
#include <QString>
#include "global.h"

QString FormatLastError(DWORD lastErr);
DWORD   NtErrToWinError(NTSTATUS status);

QString GetSystemDir();