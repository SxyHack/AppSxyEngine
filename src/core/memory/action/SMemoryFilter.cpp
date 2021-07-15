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

	auto& prevWhats = pPrevAction->GetWhatList();
	if (_FindWhats.count() < prevWhats.count())
	{
		qFatal("逻辑错误，非常致命，必须修复");
		return;
	}

	// 这层循环最多只有8-10个，不用太过担心性能, 正常情况下只有1，
	// 只有使用全部类型进行扫描时，才会出现N。
	// 先测试一波如果有问题再优化
	for (int i = 0; i < prevWhats.count(); i++) 
	{
		auto& prevWhat = prevWhats[i];
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
	auto nFoundCount = GetFoundCount();
	emit _Process->sgSearchDone(this, nFoundCount);
}
