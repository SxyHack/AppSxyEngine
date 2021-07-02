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
	// 枚举所有进程, 会发出三个信号 sgListProcessStart, sgListProcess, sgListProcessDone
	// sgListProcessStart: 在循环开始前发射
	// sgListProcess: 每发现一个进程时发射
	// sgListProcessDone: 遍历循环结束后发射
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
