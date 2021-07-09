#pragma once

#include <QThread>
#include <QThreadPool>
#include <QAtomicInt>
#include <QSemaphore>

#include "SFindWhat.h"
#include "SFindMethod.h"

class SProcess;

//
//
//
class SMemorySearch : public QThread
{
	Q_OBJECT

public:
	SMemorySearch(SProcess* pProcess);
	~SMemorySearch();

	void FindWhatReset();
	void FindWhat(const SFindWhat& what);
	void FindMethod(SFindMethod* pMethod);
	void EnableCodeRegion(bool enabled);

	void FindInRange(quint64 nStartAddr, quint64 nEndAddr);

	//
	// 取消当前搜索
	// 
	void Cancel();

protected:
	void run() override;

protected:
	FindWhatList _FindWhats;
	SFindMethod* _Method;
	SProcess*    _Process;
	QThreadPool  _ThreadPool;
	bool         _EnableCodeRegion; // 是否搜索代码页 
	bool         _EnableMapped;
	QSemaphore   _Stop;

};
