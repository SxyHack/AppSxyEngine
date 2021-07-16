#include "SElapsed.h"

SElapsed::SElapsed(const QString& qMessage)
{
	_Time.start();
	_Message = qMessage;
}

SElapsed::~SElapsed()
{
	auto elapse = _Time.elapsed();
	qDebug("%s, 耗时:%d(ms)", _Message.toUtf8().data(), elapse);
}
