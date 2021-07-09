#include "SElapsed.h"

SElapsed::SElapsed(const QString& qsLog)
{
	_Time.start();
	_Log = qsLog;
}

SElapsed::~SElapsed()
{
	qDebug("%s, 耗时:%d(ms)", _Log.toUtf8().data(), _Time.elapsed());
}
