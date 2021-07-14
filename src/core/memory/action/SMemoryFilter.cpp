#include "SMemoryFilter.h"
#include "SProcess.h"
#include "SElapsed.h"

SMemoryFilter::SMemoryFilter(SProcess* pProcess)
	: SMemoryAction(pProcess)
{
}

SMemoryFilter::~SMemoryFilter()
{
}

void SMemoryFilter::run()
{
	SElapsed elapse("执行过滤");

	SMemoryAction* pPrevAction = _Process->GetPrevAction();
	if (pPrevAction == nullptr)
	{
		qFatal("致命错误");
		return;
	}
	auto& whatLst = pPrevAction->GetWhatList();
	// 这层循环最多只有8-10个，不用太过担心性能
	for (int i = 0; i < whatLst.count(); i++) 
	{
		auto& prevWhat = whatLst[i];
		auto& what = _FindWhats[i];

		for (int n = 0; n < prevWhat.GetFoundCount(); n++)
		{
			 auto buff = prevWhat.GetBuffer(n);
			 if (buff.Update())
			 {
				 if (_Method && _Method->Match(buff, what))
				 {
					 what.AppendBuff(buff);
				 }
			 }
		}
	}

	_Process->PushMemoryAction(this);
}
