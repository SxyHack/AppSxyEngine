#include "SProcess.h"
#include "core\extras\NtExtras.h"
#include "libs\ntdll\ntdll.h"

#include <QMutex>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QDateTime>

static QMutex mxAppendThread;
static QMutex mxAppendModule;


SProcess::SProcess(const PROCESSENTRY32& entry)
	: QObject(nullptr)
	, Content(entry)
	, _Error(0)
	, _ID(entry.th32ProcessID)
	, _Handle(NULL)
{
	_Name = QString::fromWCharArray(entry.szExeFile);
	//qDebug(_Name.toUtf8().data());
}

SProcess::~SProcess()
{
	Close();
}

BOOL SProcess::Open()
{
	if (IsOpen())
	{
		return TRUE;
	}

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

BOOL SProcess::NtOpen()
{
	if (IsOpen())
	{
		return TRUE;
	}

	NTSTATUS status;
	OBJECT_ATTRIBUTES objectAttributes;
	CLIENT_ID clientId;

	/* If we don't have a PID, look it up */
	//if (dwProcessId == MAXDWORD) dwProcessId = (DWORD_PTR)CsrGetProcessId();

	/* Open a handle to the process */
	clientId.UniqueThread = NULL;
	clientId.UniqueProcess = UlongToHandle(_ID);
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
	if (_Handle == NULL)
		return TRUE;

	NT_SUCCESS(NtClose(_Handle));
	_Handle = NULL;
	return TRUE;
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

QString SProcess::GetFileName()
{
	return _Name;
}

QString SProcess::GetFilePath()
{
	if (_FilePath.length() > 0)
	{
		return _FilePath;
	}

	if (!NtOpen())
	{
		return QString();
	}

	TCHAR szFilePath[MAX_FILE_PATH_SIZE];
	if (!GetModuleFileNameEx(_Handle, NULL, szFilePath, MAX_FILE_PATH_SIZE))
	{
		return QString();
	}

	_FilePath = QString::fromWCharArray(szFilePath);

	Close();
	return _FilePath;
}


QIcon SProcess::GetIcon()
{
	auto begTime = QDateTime::currentMSecsSinceEpoch();
	QFileInfo fileInfo(GetFilePath());
	QFileIconProvider provider;
	auto icon = provider.icon(fileInfo);
	auto elapse = QDateTime::currentMSecsSinceEpoch() - begTime;
	qDebug("进程[%08d]获取图标耗时:%d(ms)", _ID, elapse);
	return icon;
}

void SProcess::AppendModule(SModule* pModule)
{
	QMutexLocker lock(&mxAppendModule);

	if (_ModuleNameMap.contains(pModule->FileName))
		return;

	_ModuleNameList.append(pModule->FileName);
	_ModuleNameMap.insert(pModule->FileName, pModule);
	_ModuleRangeMap.insert(QRange(pModule->ModBase, pModule->ModBase + pModule->ModSize - 1), pModule);
}

bool SProcess::ModuleIsExist(const QString& name)
{
	QMutexLocker lock(&mxAppendModule);

	return _ModuleNameMap.contains(name);
}

qint32 SProcess::GetModuleCount()
{
	QMutexLocker lock(&mxAppendModule);
	return _ModuleNameMap.size();
}

SModule* SProcess::GetModule(int i)
{
	QMutexLocker lock(&mxAppendModule);
	auto key = _ModuleNameList.at(i);
	return _ModuleNameMap.value(key, nullptr);
}

SModule* SProcess::GetModule(const QString& name)
{
	QMutexLocker lock(&mxAppendModule);
	return _ModuleNameMap.value(name, nullptr);
}

SModule* SProcess::GetModuleName(quint64 address, QString& name)
{
	QMutexLocker lock(&mxAppendModule);
	QRange key(address, address);

	if (_ModuleRangeMap.contains(key)) {
		auto pModule = _ModuleRangeMap.value(key);
		name = pModule->FileName;
		return pModule;
	}

	return nullptr;
}

SProcess::operator HANDLE()
{
	return _Handle;
}

bool SProcess::LoadVMRegions()
{
	if (!IsOpen())
	{
		qWarning("进程还没有关联.");
		return false;
	}

	quint64 ulQueryAddr = 0;
	quint64 ulAllocBase = 0;
	MEMORY_BASIC_INFORMATION mbi;
	ZeroMemory(&mbi, sizeof(MEMORY_BASIC_INFORMATION));

	while (::VirtualQueryEx(_Handle, (LPCVOID)ulQueryAddr, &mbi, sizeof(mbi)))
	{
		if (mbi.State == MEM_FREE) continue;

		bool bMapped = (mbi.Type == MEM_MAPPED);
		bool bReserved = (mbi.State == MEM_RESERVE);
		bool bPrevReserved = (_MemRegionList.length() > 0)
			? _MemRegionList.last().Content.State == MEM_RESERVE
			: false;

		if (bReserved || bPrevReserved || ulAllocBase != quint64(mbi.AllocationBase))
		{
			QString qsModuleName;
			SModule* lpModule = GetModuleName(ulQueryAddr, qsModuleName);
			if (lpModule == nullptr)
			{
				TCHAR szMappedName[MAX_MODULE_SIZE] = L"";
				if (bMapped && (GetMappedFileName(_Handle, mbi.AllocationBase, szMappedName, MAX_MODULE_SIZE) != 0))
				{
					qsModuleName = QString::fromWCharArray(szMappedName);
					lpModule = GetModule(qsModuleName);
				}
			}

			SMemoryRegion region(mbi, lpModule);

			_MemRegionList.append(region);
		} 
		else
		{
			if (_MemRegionList.length() > 0)
				_MemRegionList.last().Content.RegionSize += mbi.RegionSize;
		}

		quint64 ulNextRegionAddr = (quint64)mbi.BaseAddress + mbi.RegionSize;
		if (ulNextRegionAddr <= ulQueryAddr)
			break;

		ulQueryAddr = ulNextRegionAddr;
	}

	return true;
}
