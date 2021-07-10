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
	void EnableMapped(bool enabled);

	void FindInRange(quint64 nBegAddr, quint64 nEndAddr, SFindWhat& what, SModule* pModule);

	//
	// 取消当前搜索
	// 
	void Cancel();
	bool IsDone();

protected:
	void run() override;

protected:
	SWHAT_LIST   _FindWhats;
	SFindMethod* _Method;
	SProcess*    _Process;
	QThreadPool  _ThreadPool;
	bool         _EnableCodeRegion; // 是否搜索代码页 
	bool         _EnableMapped;
	bool         _Done;
	QSemaphore   _Stop;

};
