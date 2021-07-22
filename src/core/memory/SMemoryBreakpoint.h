#pragma once

#include <QObject>

class SMemoryBreakpoint : public QObject
{
	Q_OBJECT

public:
	SMemoryBreakpoint(QObject *parent);
	~SMemoryBreakpoint();
};
