#pragma once

#include <QObject>
#include <QStringList>

#include "global.h"
#include "SModule.h"


class SProcess : public QObject
{
	Q_OBJECT

public:
	SProcess(QObject *parent);
	~SProcess();

	BOOL Open(DWORD dwPID);
	BOOL NtOpen(DWORD dwPID);
	BOOL IsOpen();
	BOOL Close();

	BOOL IsWow64();

	quint64 GetID();

	//
	// 添加模块到内存
	//
	void AppendModule(SModule* pModule);
	//
	// 判断模块是否已经存在于Map数据结构中
	//
	bool ModuleIsExist(const QString& name);
	quint64 GetModuleCount();
	SModule* GetModule();
	
	//
	// 隐式转换, 返回进程句柄
	// 
	operator HANDLE();

protected:
	quint64 _ID;
	HANDLE  _Handle;
	quint32 _Error;
	QString _ErrMessage;

	RANGE_MAP_MOUDLE  _ModuleRangeMap;
	NAME_MAP_MODULE   _ModuleNameMap;
	QStringList       _ModuleNameList;  // 模块有序列表

};
