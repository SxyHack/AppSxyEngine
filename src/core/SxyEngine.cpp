#include "SxyEngine.h"
#include "SEnumProcess.h"

#include <QMutex>

static QMutex gMutex;

SxyEngine::SxyEngine()
	: QObject(nullptr)
	, _AttachProcess(nullptr)
{
}

SxyEngine::~SxyEngine()
{
}

SxyEngine& SxyEngine::Instance()
{
	QMutexLocker lock(&gMutex);

	static SxyEngine instance;
	return instance;
}

void SxyEngine::EnumProcess(const QString& filter)
{
	SEnumProcess* pWork = new SEnumProcess(filter);
	connect(pWork, &SEnumProcess::sgEnumProcess, this, &SxyEngine::sgEnumProcess);
	connect(pWork, &SEnumProcess::sgEnumProcessDone, this, &SxyEngine::sgEnumProcessDone);
	pWork->start(QThread::HighPriority);
}

void SxyEngine::AppendProcess(SProcess* pProcess)
{
	_ProcessList.append(pProcess);
}

void SxyEngine::SelectProcess(SProcess* pProcess)
{
	if (_AttachProcess)
		delete _AttachProcess;

	_AttachProcess = pProcess;
	_ProcessList.removeOne(pProcess);
}

void SxyEngine::RemoveAllProcess()
{
	for (auto it : _ProcessList)
	{
		delete it;
	}

	_ProcessList.clear();
}

LST_PROCESS& SxyEngine::GetProcessList()
{
	return _ProcessList;
}

bool SxyEngine::AttachSelectedProcess()
{
	if (!_AttachProcess->NtOpen())
	{
		return false;
	}

	_AttachProcess->ExecuteEnumModules();
	//_AttachProcess->LoadVMRegions();
	return true;
}

SProcess* SxyEngine::GetSelectedProcess()
{
	return _AttachProcess;
}

void SxyEngine::Search(EFIND_TYPE type, EFIND_METHOD method, const QString& valueA, const QString& valueB)
{
	if (_AttachProcess)
		_AttachProcess->Search(type, method, valueA, valueB);
}

void SxyEngine::Restart()
{
	if (_AttachProcess) 
		_AttachProcess->RemoveAllMemoryAction();
}

qint64 SxyEngine::QueryModuleOffset(QString& qModuleName, quint64 address)
{
	if (_AttachProcess)
	{
		auto pModule = _AttachProcess->GetModuleName(address, qModuleName);
		if (pModule)
		{
			return address - pModule->ModBase;
		}
	}

	return -1;
}
