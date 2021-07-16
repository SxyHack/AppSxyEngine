#pragma once

#include <QThread>
#include <QThreadPool>

#include "SMemoryAction.h"

class SProcess;

//
// ���������ڴ�
//
class SMemorySearch : public SMemoryAction
{
	Q_OBJECT

public:
	friend class SFindWorker;

public:
	SMemorySearch(SProcess* pProcess);
	~SMemorySearch();

	void EnableCodeRegion(bool enabled);
	void EnableMapped(bool enabled);

	void Reset();
	bool IsValidRegion(const MEMORY_BASIC_INFORMATION& mbi);

	// friend methods
protected:
	void FindInRange(quint64 nBegAddr, quint64 nEndAddr, SFindWhat& what, SModule* pModule);

protected:
	void run() override;

protected:
	QThreadPool  _ThreadPool;
	bool         _EnableCodeRegion; // �Ƿ���������ҳ 
	bool         _EnableMapped;     // �Ƿ�����MEM_MAPPED
};
