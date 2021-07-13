#pragma once
#include <QTime>

class SElapsed
{
public:
	SElapsed(const QString& qsLog);
	~SElapsed();

private:
	QTime   _Time;
	QString _Message;
};
