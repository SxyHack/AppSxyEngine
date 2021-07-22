#pragma once

#include <QThread>
#include "global.h"

typedef QList<CONTEXT> LIST_THREAD_CTX;

class SProcess;
//
// class SThread
//
class SThread : public QThread
{
	Q_OBJECT

public:
	SThread(const THREADENTRY32& entry, SProcess* pProcess);
	~SThread();

	//
	// 返回线程ID，返回成员变量(_Entry.th32ThreadID)
	//
	qint32 GetID();

protected:
	void run() override;

protected:
	SProcess*       _Process;
	THREADENTRY32   _Entry;
	quint64         _LastRIP;
	LIST_THREAD_CTX _ListRIP;
};

typedef QMap<qint32, SThread*> STHREAD_MAP;

struct SRIP
{
	SThread* pThread;
	quint64  nRIP;
};
