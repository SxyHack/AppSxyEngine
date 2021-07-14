#include "SMemoryFilter.h"
#include "SProcess.h"

SMemoryFilter::SMemoryFilter(SProcess* pProcess)
	: SMemoryAction(pProcess)
{
}

SMemoryFilter::~SMemoryFilter()
{
}

void SMemoryFilter::run()
{
	SMemoryAction* pPrevAction = _Process->GetPrevAction();
	if (pPrevAction == nullptr)
	{
		qFatal("ÖÂÃü´íÎó");
		return;
	}
	auto& whatLst = pPrevAction->GetWhatList();
	for (int i = 0; i < whatLst.count(); i++)
	{
		auto& prevWhat = whatLst[i];
		auto& what = _FindWhats[i];

		for (int n = 0; n < prevWhat.GetFoundCount(); n++)
		{
			 auto& buff = prevWhat.GetBuffer(n);
			 if (_Method && _Method->Match(buff, what))
			 {
				 what.AppendBuff(buff);
			 }
		}
	}
}
