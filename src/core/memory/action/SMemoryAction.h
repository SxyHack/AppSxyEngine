#pragma once

#include <QThread>
#include <QThreadPool>
#include <QSemaphore>
#include <QList>

#include "global.h"
#include "SFindWhat.h"
#include "SFindMethod.h"

class SProcess;

class SMemoryAction : public QThread
{
	Q_OBJECT

public:
	SMemoryAction(SProcess* pProcess);
	~SMemoryAction();
	//
	// 取消当前搜索
	// 
	void Cancel();

	void FindWhatReset();
	void FindWhat(const SFindWhat& what);
	void FindMethod(SFindMethod* pMethod);

	SWHAT_LIST& GetWhatList();
	quint32 GetFoundCount();

	// Get _VirtualMemorySearchedSize
	//quint64 GetSearchedSize();
	// Get _VirtualMemorySize
	//quint64 GetMemorySize();

signals:
	//void sgSearchDone(quint32 count);
	void sgSearchDone(SMemoryAction* pAction, quint32 count);

protected:
	SWHAT_LIST   _FindWhats;
	SFindMethod* _Method;
	SProcess*    _Process;
	quint32      _NumberOfProcessors;
	quint64      _ProcessBegAddress;
	quint64      _ProcessEndAddress;
	//quint64      _VirtualMemorySize;          // 虚拟内存空间总大小
	//quint64      _VirtualMemorySearchedSize;  // 虚拟内存空间已搜索大小
	QSemaphore   _Stop;
};

typedef QList<SMemoryAction*> SMEMORY_ACTION_LIST;