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
	SEnumProcess* pEnumProcess = new SEnumProcess(filter);
	connect(pEnumProcess, &SEnumProcess::sgEnumProcessDone, this, &SxyEngine::sgEnumProcessDone);
	pEnumProcess->start(QThread::HighPriority);
}

void SxyEngine::AppendProcess(SProcess* pProcess)
{
	_LstProcess.append(pProcess);
}

void SxyEngine::SelectProcess(SProcess* pProcess)
{
	_AttachProcess = pProcess;
}

LST_PROCESS& SxyEngine::GetProcessList()
{
	return _LstProcess;
}
