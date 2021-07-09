#include "SFindWorker.h"
#include "../SMemorySearch.h"

SFindWorker::SFindWorker(SMemorySearch* pSearch, quint64 begAddr, quint64 endAddr)
	: QObject(nullptr)
	, _Search(pSearch)
	, _FindBegAddress(begAddr)
	, _FindEndAddress(endAddr)
{
}

SFindWorker::~SFindWorker()
{
	//qDebug("~[%p, %p]", _FindBegAddress, _FindEndAddress);
}

void SFindWorker::run()
{
	_Search->FindInRange(_FindBegAddress, _FindEndAddress);
}
