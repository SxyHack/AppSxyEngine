#pragma once

#include <QObject>
#include <QMap>
#include <QVariant>

#include "memory_define.h"

class SModule;
class SFindWhat;
class SProcess;
//
// �ڴ����ҵ����ڴ��
//
class SMemoryBuffer : public QObject
{
	Q_OBJECT

public:
	SMemoryBuffer();
	SMemoryBuffer(quint64 nAddr, char* pContent, SFindWhat* pWhat, SModule* pModule, SProcess* pProcess);
	SMemoryBuffer(const SMemoryBuffer& src);
	~SMemoryBuffer();

	SMemoryBuffer& operator=(const SMemoryBuffer& src);

	QString ToString() const;

	//
	// ���¶�ȡ�ڴ���ֵ
	//
	bool Update();


public:
	static bool IsCanonicalAddress(quint64 address);

public:
	quint64  Address;
	QVariant Content;

protected:
	SProcess*  _Process;
	SModule*   _Module;
	SFindWhat* _What;
};

typedef QMap<quint64, SMemoryBuffer> MEMORY_BUFF_MAP;

Q_DECLARE_METATYPE(SMemoryBuffer);
