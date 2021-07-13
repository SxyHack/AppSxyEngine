#pragma once

#include <QObject>
#include <QMap>
#include <QVariant>

#include "memory_define.h"

class SModule;
class SFindWhat;

//
// 内存中找到的内存块
//
class SMemoryBuffer : public QObject
{
	Q_OBJECT

public:
	SMemoryBuffer();
	SMemoryBuffer(quint64 nAddr, char* pBuffer, SFindWhat* pWhat, SModule* pModule);
	SMemoryBuffer(const SMemoryBuffer& src);
	~SMemoryBuffer();

	SMemoryBuffer& operator=(const SMemoryBuffer& src);

	QString ToString() const;

	static bool IsCanonicalAddress(quint64 address);
	static long GetBufferLength(EFIND_TYPE value);

public:
	quint64  Address;
	QVariant Content;

protected:
	SModule*   _Module;
	SFindWhat* _What;
};

typedef QMap<quint64, SMemoryBuffer> MEMORY_BUFF_MAP;
