#pragma once

#include <QObject>
#include <QVariant>
#include <QList>

#include "memory_define.h"
#include "SMemoryBuffer.h"

//
// ’“ ≤√¥
// 
class SFindWhat 
{
public:
	SFindWhat();
	SFindWhat(quint8 v);
	SFindWhat(quint8 a, quint8 b);
	SFindWhat(quint16 v);
	SFindWhat(quint16 a, quint16 b);
	SFindWhat(quint32 v);
	SFindWhat(quint32 a, quint32 b);
	SFindWhat(quint64 v);
	SFindWhat(quint64 a, quint64 b);
	SFindWhat(float v);
	SFindWhat(float a, float b);
	SFindWhat(double v);
	SFindWhat(double a, double b);
	SFindWhat(QString txt);
	SFindWhat(const SFindWhat& src);
	~SFindWhat();

	void AppendBuff(const SMemoryBuffer& buff);
	long GetFoundCount();

public:
	static SFindWhat Create(const QString& a, const QString& b, EFIND_TYPE type);

public:
	QVariant A;
	QVariant B;
	quint32 Size;

protected:
	MEMORY_BUFF_MAP  _FoundBuffMap;
	QList<quint64>   _FoundAddressList;

};

typedef QList<SFindWhat> SWHAT_LIST;
