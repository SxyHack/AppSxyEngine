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
	SElapsed elapse("ִ�й���");

	SMemoryAction* pPrevAction = _Process->GetPrevAction();
	if (pPrevAction == nullptr)
	{
		qFatal("��������");
		return;
	}
	auto& whatLst = pPrevAction->GetWhatList();
	// ���ѭ�����ֻ��8-10��������̫����������
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
