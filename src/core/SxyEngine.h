#pragma once

#include <QObject>
#include "SProcess.h"


//
//
//
class SxyEngine : public QObject
{
	Q_OBJECT

public:
	~SxyEngine();

	static SxyEngine& Instance();

	//
	// ö�����н���, �ᷢ�������ź� sgListProcessStart, sgListProcess, sgListProcessDone
	// sgListProcessStart: ��ѭ����ʼǰ����
	// sgListProcess: ÿ����һ������ʱ����
	// sgListProcessDone: ����ѭ����������
	//
	void EnumProcess(const QString& filter = QString());
	void AppendProcess(SProcess* pProcess);  // 
	void SelectProcess(SProcess* pProcess);
	void RemoveAllProcess();
	LST_PROCESS& GetProcessList();


signals:
	//void sgEnumProcessStart();
	void sgEnumProcess(qint32, SProcess*);
	void sgEnumProcessDone();

private:
	SxyEngine();

	LST_PROCESS _ProcessList;
	SProcess* _AttachProcess;
};



#define SEngine SxyEngine::Instance()
