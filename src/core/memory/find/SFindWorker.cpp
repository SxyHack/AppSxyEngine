#include "SFindWorker.h"
#include "../SMemorySearch.h"

SFindWorker::SFindWorker(SMemorySearch* pSearch, SModule* pModule, const SFindWhat& what, quint64 begAddr, quint64 endAddr)
	: QObject(nullptr)
	, _Search(pSearch)
	, _FindBegAddress(begAddr)
	, _FindEndAddress(endAddr)
	, _What(what)
	, _Module(pModule)
{
	qDebug("建立并行处理: [%p, %p]", begAddr, endAddr);
}

SFindWorker::~SFindWorker()
{
}

void SFindWorker::run()
{
	_Search->FindInRange(_FindBegAddress, _FindEndAddress, _What, _Module);
}
