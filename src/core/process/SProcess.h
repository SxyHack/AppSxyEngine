#pragma once

#include <QObject>
#include <QStringList>
#include <QIcon>

#include "global.h"
#include "SModule.h"
#include "SMemoryRegion.h"

class SProcess : public QObject
{
	Q_OBJECT

public:
	SProcess(const PROCESSENTRY32& entry);
	~SProcess();

	BOOL Open();
	BOOL NtOpen();
	BOOL IsOpen();
	BOOL Close();
	BOOL IsWow64();

	quint64 GetID();
	QString GetFileName();
	QString GetFilePath();
	QIcon GetIcon();
	//
	// 添加模块到Map数据结构
	//
	void AppendModule(SModule* pModule);
	//
	// 判断模块是否已经存在于Map数据结构中
	//
	bool ModuleIsExist(const QString& name);
	qint32 GetModuleCount();
	SModule* GetModule(int i);
	SModule* GetModule(const QString& name);
	SModule* GetModuleName(quint64 address, QString& name);
	//
	// 隐式转换, 返回进程句柄
	// 
	operator HANDLE();

	// 
	// 读取虚拟内存页
	//
	bool LoadVMRegions();

public:
	PROCESSENTRY32 Content;

protected:
	quint64 _ID;
	HANDLE  _Handle;
	QString _Name;
	QString _FilePath;

	quint32 _Error;
	QString _ErrMessage;

	RANGE_MAP_MOUDLE  _ModuleRangeMap;
	NAME_MAP_MODULE   _ModuleNameMap;
	QStringList       _ModuleNameList;  // 模块有序列表
	LST_MEMORY_REGION _MemRegionList;   // 内存页列表
};


typedef QList<SProcess*> LST_PROCESS;