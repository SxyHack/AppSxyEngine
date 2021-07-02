#pragma once

#include <QByteArray>

class Random
{
public:
	static QByteArray GenerateString(qint32 count, int sp = 0);
};
