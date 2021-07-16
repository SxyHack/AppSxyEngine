#pragma once

#include "SMemoryAction.h"

class SProcess;

class SMemoryFilter : public SMemoryAction
{
	Q_OBJECT

public:
	SMemoryFilter(SProcess* pProcess);
	~SMemoryFilter();

protected:
	void run() override;
};
