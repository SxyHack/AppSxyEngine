#pragma once

#include <QObject>
#include <QRunnable>

class SMemoryFilter;

class SFilteWorker : public QObject, public QRunnable
{
	Q_OBJECT

public:
	SFilteWorker(SMemoryFilter* filter);
	~SFilteWorker();

protected:
	SMemoryFilter* _Filter;
};
