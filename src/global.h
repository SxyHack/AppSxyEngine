#pragma once

// Windows Header Files:
#include <windows.h>

#include <imagehlp.h>
#include <wincrypt.h>
#include <psapi.h>
#include <shellapi.h>
#include <tlhelp32.h>

// Qt Header Files:
#include <QTime>

// Other Header Files:
#include "utility/assert.h"


// defines:
#define MAX_FILE_PATH_SIZE 1024
#define ENABLE_LOG_PE_SECTION 0
#define ENABLE_LOG_MODULE     0