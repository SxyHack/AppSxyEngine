#pragma once

#include <QObject>
#include <QThreadPool>

#include "SProcess.h"
#include "SMemoryBuffer.h"

class SAbstractAction;

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
	// 枚举所有进程, 会发出信号: sgEnumProcess, sgEnumProcessDone 
	// sgListProcess: 每发现一个进程时发射
	// sgListProcessDone: 遍历循环结束后发射
	//
	void EnumProcess(const QString& filter = QString());
	//
	// 添加进程到_ProcessList队列
	//
	void AppendProcess(SProcess* pProcess);
	void SelectProcess(SProcess* pProcess);
	void RemoveAllProcess();
	LST_PROCESS& GetProcessList();
	bool AttachSelectedProcess();
	SProcess* GetSelectedProcess();

	// TODO：枚举窗口

	//
	// 搜索数值
	//
	void Search(EFIND_TYPE type, EFIND_METHOD compare, 
		const QString& valueA, const QString& valueB = QString());

signals:
	void sgEnumProcess(qint32, SProcess*);
	void sgEnumProcessDone();

private:
	SxyEngine();

	LST_PROCESS _ProcessList;
	SProcess* _AttachProcess;
};



#define SEngine SxyEngine::Instance()
