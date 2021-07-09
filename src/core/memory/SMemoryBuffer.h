#pragma once

#include <QObject>
#include <QMap>

#include "memory_define.h"


class SModule;

//
// 内存中找到的内存块
//
class SMemoryBuffer : public QObject
{
	Q_OBJECT

public:
	SMemoryBuffer(quint64 addr, const QByteArray& bytes, SModule* pModule);
	SMemoryBuffer(const SMemoryBuffer& src);
	~SMemoryBuffer();

	SMemoryBuffer& operator=(const SMemoryBuffer& src);

	static bool IsCanonicalAddress(quint64 address);
	static long GetBufferLength(EFIND_TYPE value);

public:
	quint64 Address;
	QByteArray Content;

protected:
	SModule* _Module;
};

typedef QMap<quint64, SMemoryBuffer> MEMORY_BUFF_MAP;
