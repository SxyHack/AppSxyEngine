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
	// ���ģ�鵽Map���ݽṹ
	//
	void AppendModule(SModule* pModule);
	//
	// �ж�ģ���Ƿ��Ѿ�������Map���ݽṹ��
	//
	bool ModuleIsExist(const QString& name);
	qint32 GetModuleCount();
	SModule* GetModule(int i);
	SModule* GetModule(const QString& name);
	SModule* GetModuleName(quint64 address, QString& name);
	//
	// ��ʽת��, ���ؽ��̾��
	// 
	operator HANDLE();

	// 
	// ��ȡ�����ڴ�ҳ
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
	QStringList       _ModuleNameList;  // ģ�������б�
	LST_MEMORY_REGION _MemRegionList;   // �ڴ�ҳ�б�
};


typedef QList<SProcess*> LST_PROCESS;