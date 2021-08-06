#pragma once

#include <QThread>
#include "global.h"
#include "libs/ntdll/ntdll.h"
#include "SThreadRiper.h"

typedef QList<CONTEXT> LIST_RIP;

class SProcess;
//
// class SThread
//
class SThread : public QThread
{
	Q_OBJECT

public:
	//SThread(const THREADENTRY32& entry, SProcess* pProcess);
	SThread(const PSYSTEM_THREAD_INFORMATION pData, SProcess* pProcess);
	~SThread();

	//
	// �����߳�ID�����س�Ա����(_Entry.th32ThreadID)
	//
	qint64 GetID();
	qint32 GetIndex();
	
	void SetIndex(qint32 nIndex);

	//
	// �ҳ���ʲô������nAddress��ַ
	//
	void FindOpcodeAccess(quint64 nAddress);

protected:
	void run() override;

protected:
	SProcess*       _Process;
	LIST_RIP        _ListRIP;
	SThreadRiper    _Riper;
	qint32          _Index;
	quint64         _AddressOfOpcodeAccess;  // ִ�д�����ʵĵ�ַ
	PSYSTEM_THREAD_INFORMATION _Data;
};

typedef QMap<qint64, SThread*> STHREAD_MAP;