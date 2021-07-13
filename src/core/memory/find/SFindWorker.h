#pragma once

#include <QObject>
#include <QRunnable>
#include "SFindWhat.h"

class SMemorySearch;

class SFindWorker : public QObject, public QRunnable
{
	friend class SMemorySearch;

public:
	SFindWorker(SMemorySearch* pSearch, SModule* pModule, SFindWhat* what, quint64 begAddr, quint64 endAddr);
	~SFindWorker();

protected:
	void run() override;

protected:
	SMemorySearch* _Search;
	quint64   _FindBegAddress;
	quint64   _FindEndAddress;
	SFindWhat* _What;
	SModule*  _Module;
};
 