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
	// 重新读取内存数值
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
