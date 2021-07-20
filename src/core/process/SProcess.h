#pragma once

#include <QObject>
#include <QStringList>
#include <QIcon>
#include <QMutex>

#include "SModule.h"
#include "SEnumModule.h"
#include "SMemoryRegion.h"
#include "SMemorySearch.h"
#include "SMemoryFilter.h"

#include "SElapsed.h"

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
	SModule* GetModule(quint64 address);
	SModule* GetModuleName(quint64 address, QString& name);
	void ExecuteEnumModules();

	// 
	// 添加模块到扫描白名单
	// 
	void AppendModuleToWhitelist(SModule* pModule);
	//
	// 返回模块是否扫描白名单的
	//
	bool InWhitelist(const QString& qsModuleName);

	//
	// 隐式转换, 返回进程句柄
	// 
	operator HANDLE();
	//
	// 获取进程句柄
	//
	HANDLE GetHandle();

	// 
	// 读取虚拟内存页
	//
	bool LoadVMRegions();
	//
	// @{废弃} 读取虚拟内存
	// [OUT] bytes  返回字节
	// [IN] address 要读取的内存地址
	// [IN] length  要读取的内存长度
	// [RETURN] bool 返回函数是否成功
	//
	bool ReadMemory(QByteArray& bytes, LPVOID address, quint32 length);
	//
	// 返回是否代码页
	// [IN] address  传入内存开始地址
	// [IN] length   传入内存的长度
	// [RETURN] bool 是代码返回true，否则返回false
	//
	bool IsCodeRegion(const MEMORY_BASIC_INFORMATION& mbi);

	//
	// 搜索
	//
	void Search(EFIND_TYPE type, EFIND_METHOD method, const QString& a, const QString& b);
	void PushMemoryAction(SMemoryAction* pAction);
	void RemoveAllMemoryAction();
	SMemoryAction* GetPrevAction();

	// 返回首次搜索指针
	SMemoryAction* GetBaseAction();

signals:
	void sgSearchDone(SMemoryAction* pAction, quint32 count);

public:
	PROCESSENTRY32 TlhEntry32;
	quint32 NumberOfVirtualMemory;  // 进程的虚拟内存总大小
	quint32 NumberOfSearch;         // 进程的搜索进度

protected:
	quint64 _ID;
	HANDLE  _Handle;
	QString _Name;
	QString _FilePath;
	QIcon   _FileICON;

	quint32 _Error;
	QString _ErrMessage;

	RANGE_MAP_MOUDLE   _ModuleRangeMap;
	NAME_MAP_MODULE    _ModuleNameMap;
	NAME_MAP_MODULE    _ModuleWhiteList;  // 内存扫描白名单
	LIST_MEMORY_REGION _MemRegionList;    // 内存页列表
	QStringList        _ModuleNameList;   // 模块有序列表
	SEnumModule        _EnumModules;

	// 内存相关
	SMEMORY_ACTION_LIST _Actions;
	QMutex _ActionMutex;
};


typedef QList<SProcess*> LST_PROCESS;