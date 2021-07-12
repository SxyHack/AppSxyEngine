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
	, _EnumModules(this)
	, _Search(this)
{
	_Name = QString::fromWCharArray(entry.szExeFile);

	// connects
	connect(&_Search, &SMemorySearch::sgSearchDone, this, &SProcess::sgSearchDone);
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
	if (!::IsWow64Process(_Handle, &bIsWow64))
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
	if (_FileICON.isNull())
	{
		QFileInfo fileInfo(GetFilePath());
		QFileIconProvider provider;
		_FileICON = provider.icon(fileInfo);
	}

	return _FileICON;
}

void SProcess::AppendModule(SModule* pModule)
{
	QMutexLocker lock(&mxAppendModule);

	if (_ModuleNameMap.contains(pModule->FileName))
		return;

	_ModuleNameList.append(pModule->FileName);
	_ModuleNameMap.insert(pModule->FileName, pModule);
	_ModuleRangeMap.insert(std::make_pair(QRange(pModule->ModBase, pModule->ModBase + pModule->ModSize - 1), pModule));
	//_ModuleRangeMap.insert(QRange(pModule->ModBase, pModule->ModBase + pModule->ModSize - 1), pModule);
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

SModule* SProcess::GetModule(quint64 address)
{
	QMutexLocker lock(&mxAppendModule);
	QRange key(address, address);

	auto found = _ModuleRangeMap.find(key);
	if (found != _ModuleRangeMap.end())
	{
		return found->second;
	}

	return nullptr;
}

SModule* SProcess::GetModuleName(quint64 address, QString& name)
{
	QMutexLocker lock(&mxAppendModule);
	QRange key(address, address);

	auto found = _ModuleRangeMap.find(key);
	if (found != _ModuleRangeMap.end())
	{
		name = found->second->FileName;
		return found->second;
	}

	//if (_ModuleRangeMap.contains(key)) {
	//	auto pModule = _ModuleRangeMap.value(key);
	//	name = pModule->FileName;
	//	return pModule;
	//}

	return nullptr;
}

void SProcess::ExecuteEnumModules()
{
	if (_EnumModules.isRunning())
		return;

	_EnumModules.start(QThread::NormalPriority);
	_EnumModules.WaitForInit();
}

void SProcess::AppendModuleToWhitelist(SModule* pModule)
{
	_ModuleWhiteList.insert(pModule->FileName, pModule);
}

bool SProcess::InWhitelist(const QString& qsModuleName)
{
	return _ModuleWhiteList.contains(qsModuleName);
}

SProcess::operator HANDLE()
{
	return _Handle;
}

HANDLE SProcess::GetHandle()
{
	return _Handle;
}

bool SProcess::LoadVMRegions()
{
	SElapsed elapse("LoadVMRegions");

	if (!IsOpen())
	{
		qWarning("进程还没有关联.");
		return false;
	}

	quint64 ulQueryAddr = 0;
	quint64 ulAllocBase = 0;
	MEMORY_BASIC_INFORMATION mbi;
	ZeroMemory(&mbi, sizeof(MEMORY_BASIC_INFORMATION));

	while (::VirtualQueryEx(_Handle, (LPCVOID)ulQueryAddr, &mbi, sizeof(MEMORY_BASIC_INFORMATION)))
	{
		QString qsModuleName;
		auto qsAllocMemProtect = FormatMemProtection(mbi.AllocationProtect);
		auto qsMemProtect = FormatMemProtection(mbi.Protect);
		auto qsMemState = FormatMemState(mbi.State);
		auto qsMemType = FormatMemType(mbi.Type);

		if (mbi.State == MEM_FREE)
			goto LOOP_END;
		bool bMapped = (mbi.Type == MEM_MAPPED);
		bool bReserved = (mbi.State == MEM_RESERVE);
		bool bPrevReserved = (_MemRegionList.length() > 0)
			? _MemRegionList.last().Content.State == MEM_RESERVE
			: false;

		TCHAR szModuleName[MAX_MODULE_SIZE] = { 0 };
		::GetModuleBaseName(_Handle, (HMODULE)mbi.AllocationBase, szModuleName, MAX_MODULE_SIZE);
		qsModuleName = QString::fromWCharArray(szModuleName);
		qDebug("Region: %s %p %p %p %s %s %s %s",
			qsModuleName.toUtf8().data(),
			mbi.BaseAddress,
			mbi.AllocationBase,
			quint64(mbi.BaseAddress) + mbi.RegionSize,
			qsAllocMemProtect.toUtf8().data(),
			qsMemProtect.toUtf8().data(),
			qsMemState.toUtf8().data(),
			qsMemType.toUtf8().data());

	LOOP_END:
		quint64 ulNextRegionAddr = (quint64)mbi.BaseAddress + mbi.RegionSize;
		if (ulNextRegionAddr <= ulQueryAddr)
			break;

		ulQueryAddr = ulNextRegionAddr;
	}

	return true;
}

bool SProcess::ReadMemory(QByteArray& bytes, LPVOID address, quint32 length)
{
	//SElapsed elapse("ReadMemory");
	quint64 nReadedLength = 0;
	char* pBuffer = new char[length];
	ZeroMemory(pBuffer, length);
	if (!::ReadProcessMemory(_Handle, address, pBuffer, length, &nReadedLength))
	{
		return false;
	}

	bytes.append(pBuffer, nReadedLength);
	delete[] pBuffer;
	return true;
}

bool SProcess::IsCodeRegion(const MEMORY_BASIC_INFORMATION& mbi)
{
	bool bExecute = (mbi.Protect & PAGE_EXECUTE) ||
		(mbi.Protect & PAGE_EXECUTE_READ) ||
		(mbi.Protect & PAGE_EXECUTE_READWRITE) ||
		(mbi.Protect & PAGE_EXECUTE_WRITECOPY);

	auto pModule = GetModule((quint64)mbi.BaseAddress);
	if (pModule == nullptr)
		return false;

	return pModule->IsCodeRegion((quint64)mbi.BaseAddress) || bExecute;
}

void SProcess::Search(EFIND_TYPE type, EFIND_METHOD method, const QString& a, const QString& b)
{
	if (_Search.IsDone())
	{

	}
	else
	{
		_Search.FindMethod(SFindMethod::Create(method));
		if (type == EFIND_TYPE::All)
		{

		}
		else
		{
			_Search.FindWhat(SFindWhat::Create(a, b, type));
		}

		_Search.start();
	}
}

void SProcess::GetSearchProgress(quint64& readed, quint64& total)
{
	readed = _Search.GetReadedBytes();
	total = _Search.GetTotalBytes();
}

SMemorySearch& SProcess::GetSearch()
{
	return _Search;
}
