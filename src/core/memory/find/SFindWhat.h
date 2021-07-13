#pragma once

#include <QObject>
#include <QVariant>
#include <QList>
#include <QMutex>

#include "memory_define.h"
#include "SMemoryBuffer.h"

//
// ’“ ≤√¥
// 
class SFindWhat 
{
public:
	SFindWhat();
	//SFindWhat(quint8 v);
	//SFindWhat(quint16 v);
	//SFindWhat(quint32 v);
	//SFindWhat(quint64 v);
	//SFindWhat(float v);
	//SFindWhat(double v);

	SFindWhat(quint8 a, quint8 b, EFIND_TYPE type);
	SFindWhat(quint16 a, quint16 b, EFIND_TYPE type);
	SFindWhat(quint32 a, quint32 b, EFIND_TYPE type);
	SFindWhat(quint64 a, quint64 b, EFIND_TYPE type);
	SFindWhat(float a, float b, EFIND_TYPE type);
	SFindWhat(double a, double b, EFIND_TYPE type);
	SFindWhat(QString txt, EFIND_TYPE type);
	SFindWhat(const SFindWhat& src);
	~SFindWhat();

	void AppendBuff(const SMemoryBuffer& buff);
	long GetFoundCount();

	const SMemoryBuffer GetBuffer(int i);

public:
	static SFindWhat Create(const QString& a, const QString& b, EFIND_TYPE type);

public:
	QVariant A;
	QVariant B;
	quint32 Size;
	EFIND_TYPE Type;

protected:
	MEMORY_BUFF_MAP  _FoundBuffMap;
	QList<quint64>   _FoundAddressList;
	QMutex           _ListLock;
};

typedef QList<SFindWhat> SWHAT_LIST;
