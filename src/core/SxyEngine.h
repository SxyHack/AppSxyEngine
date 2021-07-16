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
	void Search(EFIND_TYPE type, 
		EFIND_METHOD compare, 
		const QString& valueA, 
		const QString& valueB = QString());

	//
	// 清空搜索结果，重新开始新的搜索
	//
	void Restart();

	//
	// 查询地址是否是模块的静态地址
	// [OUT] qModuleName 返回模块名字，如果为空，则是私有内存页
	// [IN]  address 输入要查询的地址
	// [RETURN] 如果是私有内存页返回-1, 否则返回模块镜像的偏移
	// 
	qint64 QueryModuleOffset(QString& qModuleName, quint64 address);

signals:
	void sgEnumProcess(qint32, SProcess*);
	void sgEnumProcessDone();

private:
	SxyEngine();

	LST_PROCESS _ProcessList;
	SProcess* _AttachProcess;
};



#define SEngine SxyEngine::Instance()
