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
	// ���ģ�鵽�ڴ�
	//
	void AppendModule(SModule* pModule);
	//
	// �ж�ģ���Ƿ��Ѿ�������Map���ݽṹ��
	//
	bool ModuleIsExist(const QString& name);
	quint64 GetModuleCount();
	SModule* GetModule();
	
	//
	// ��ʽת��, ���ؽ��̾��
	// 
	operator HANDLE();

protected:
	quint64 _ID;
	HANDLE  _Handle;
	quint32 _Error;
	QString _ErrMessage;

	RANGE_MAP_MOUDLE  _ModuleRangeMap;
	NAME_MAP_MODULE   _ModuleNameMap;
	QStringList       _ModuleNameList;  // ģ�������б�

};
