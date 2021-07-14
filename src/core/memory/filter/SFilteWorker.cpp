#include "SFilteWorker.h"
#include "../action/SMemoryFilter.h"

SFilteWorker::SFilteWorker(SMemoryFilter* filter)
	: QObject(nullptr)
	, _Filter(filter)
{
}

SFilteWorker::~SFilteWorker()
{
}
