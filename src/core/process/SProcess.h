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
	SModule* GetModule(quint64 address);
	SModule* GetModuleName(quint64 address, QString& name);
	void ExecuteEnumModules();

	// 
	// ���ģ�鵽ɨ�������
	// 
	void AppendModuleToWhitelist(SModule* pModule);
	//
	// ����ģ���Ƿ�ɨ���������
	//
	bool InWhitelist(const QString& qsModuleName);

	//
	// ��ʽת��, ���ؽ��̾��
	// 
	operator HANDLE();
	//
	// ��ȡ���̾��
	//
	HANDLE GetHandle();

	// 
	// ��ȡ�����ڴ�ҳ
	//
	bool LoadVMRegions();
	//
	// @{����} ��ȡ�����ڴ�
	// [OUT] bytes  �����ֽ�
	// [IN] address Ҫ��ȡ���ڴ��ַ
	// [IN] length  Ҫ��ȡ���ڴ泤��
	// [RETURN] bool ���غ����Ƿ�ɹ�
	//
	bool ReadMemory(QByteArray& bytes, LPVOID address, quint32 length);
	//
	// �����Ƿ����ҳ
	// [IN] address  �����ڴ濪ʼ��ַ
	// [IN] length   �����ڴ�ĳ���
	// [RETURN] bool �Ǵ��뷵��true�����򷵻�false
	//
	bool IsCodeRegion(const MEMORY_BASIC_INFORMATION& mbi);

	//
	// ����
	//
	void Search(EFIND_TYPE type, EFIND_METHOD method, const QString& a, const QString& b);
	void PushMemoryAction(SMemoryAction* pAction);
	void RemoveAllMemoryAction();
	SMemoryAction* GetPrevAction();

	// �����״�����ָ��
	SMemoryAction* GetBaseAction();

signals:
	void sgSearchDone(SMemoryAction* pAction, quint32 count);

public:
	PROCESSENTRY32 TlhEntry32;
	quint32 NumberOfVirtualMemory;  // ���̵������ڴ��ܴ�С
	quint32 NumberOfSearch;         // ���̵���������

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
	NAME_MAP_MODULE    _ModuleWhiteList;  // �ڴ�ɨ�������
	LIST_MEMORY_REGION _MemRegionList;    // �ڴ�ҳ�б�
	QStringList        _ModuleNameList;   // ģ�������б�
	SEnumModule        _EnumModules;

	// �ڴ����
	SMEMORY_ACTION_LIST _Actions;
	QMutex _ActionMutex;
};


typedef QList<SProcess*> LST_PROCESS;