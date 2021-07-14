#pragma once

#include <QObject>
#include <QMap>
#include <QVariant>

#include "memory_define.h"

class SModule;
class SFindWhat;
class SProcess;
//
// 内存中找到的内存块
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
	//
	//
	bool Update();

	static bool IsCanonicalAddress(quint64 address);
	static long GetBufferLength(EFIND_TYPE value);

public:
	quint64  Address;
	QVariant Content;

protected:
	SModule*   _Module;
	SFindWhat* _What;
	SProcess* _Process;
};

typedef QMap<quint64, SMemoryBuffer> MEMORY_BUFF_MAP;
