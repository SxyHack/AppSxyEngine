#include "SxyEngine.h"
#include "SEnumProcess.h"

#include <QMutex>

static QMutex gMutex;

SxyEngine::SxyEngine()
	: QObject(nullptr)
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
	_AttachProcess = pProcess;
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

	if (!_AttachProcess->LoadVMRegions())
	{
		return false;
	}

	return true;
}
