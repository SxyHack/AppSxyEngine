#pragma once

#include <QThread>
#include <QThreadPool>
#include <QAtomicInt>
#include <QSemaphore>

#include "SFindWhat.h"
#include "SFindMethod.h"
#include "global.h"

class SProcess;

//
// ËÑË÷ÐéÄâÄÚ´æ
//
class SMemorySearch : public QThread
{
	Q_OBJECT
public:
	friend class SMemorySearchFilter;

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
	// È¡Ïûµ±Ç°ËÑË÷
	// 
	void Cancel();
	void Reset();
	bool IsDone();
	bool IsValidRegion(const MEMORY_BASIC_INFORMATION& mbi);

	SWHAT_LIST& GetWhatList();
	quint32 GetWhatCount();

	quint64 GetReadedBytes();
	quint64 GetTotalBytes();

protected:
	void run() override;

signals:
	void sgSearchDone(quint32 count);

protected:
	SWHAT_LIST   _FindWhats;
	SFindMethod* _Method;
	SProcess*    _Process;
	QThreadPool  _ThreadPool;
	bool         _EnableCodeRegion; // ÊÇ·ñËÑË÷´úÂëÒ³ 
	bool         _EnableMapped;     // ÊÇ·ñËÑË÷MEM_MAPPED
	bool         _Done;
	QSemaphore   _Stop;
	quint32      _NumberOfProcessors;
	quint64      _ProcessBegAddress;
	quint64      _ProcessEndAddress;
	quint64      _ProcessTotalBytes;
	quint64      _ProcessReadedBytes;
};



class SMemorySearchFilter : public QThread
{
public:
	SMemorySearchFilter(SMemorySearch* pSearch);
	~SMemorySearchFilter();

protected:
	void run() override;

protected:
	SMemorySearch* _Search;
};