#include "SFindWhat.h"

SFindWhat::SFindWhat(quint8 a, quint8 b, EFIND_TYPE type)
	: A(a)
	, B(b)
	, Size(sizeof(quint8))
	, Type(type)
{
}

SFindWhat::SFindWhat(quint16 a, quint16 b, EFIND_TYPE type)
	: A(a)
	, B(b)
	, Size(sizeof(quint16))
	, Type(type)
{
}

SFindWhat::SFindWhat(quint32 a, quint32 b, EFIND_TYPE type)
	: A(a)
	, B(b)
	, Size(sizeof(quint32))
	, Type(type)
{
}

SFindWhat::SFindWhat(quint64 a, quint64 b, EFIND_TYPE type)
	: A(a)
	, B(b)
	, Size(sizeof(quint64))
	, Type(type)
{

}

SFindWhat::SFindWhat(float a, float b, EFIND_TYPE type)
	: A(a)
	, B(b)
	, Size(sizeof(float))
	, Type(type)
{

}

SFindWhat::SFindWhat(double a, double b, EFIND_TYPE type)
	: A(a)
	, B(b)
	, Size(sizeof(double))
	, Type(type)
{

}

SFindWhat::SFindWhat(QString txt, EFIND_TYPE type)
	: A(txt)
	, Size(txt.length())
	, Type(type)
{
}

SFindWhat::SFindWhat(const SFindWhat& src)
{
	A = src.A;
	B = src.B;
	Size = src.Size;
	Type = src.Type;
}

SFindWhat::SFindWhat()
{
	Size = 0;
	Type = EFIND_TYPE::Byte_4;
}

SFindWhat::~SFindWhat()
{
}

void SFindWhat::AppendBuff(const SMemoryBuffer& buff)
{
	QMutexLocker ml(&_ListLock);
	_FoundAddressList.append(buff.Address);
	_FoundBuffMap.insert(buff.Address, buff);
}

long SFindWhat::GetFoundCount()
{
	return _FoundAddressList.count();
}

const SMemoryBuffer SFindWhat::GetBuffer(int i) 
{
	return _FoundBuffMap.value(_FoundAddressList[i]);
}

SFindWhat SFindWhat::Create(const QString& a, const QString& b, EFIND_TYPE type)
{
	switch (type)
	{
	case EFIND_TYPE::Byte:
		return SFindWhat((quint8)a.toUInt(), (quint8)b.toUInt(), type);
	case EFIND_TYPE::Byte_2:
		return SFindWhat((quint16)a.toUInt(), (quint16)b.toUInt(), type);
	case EFIND_TYPE::Byte_4:
		return SFindWhat((quint32)a.toUInt(), (quint32)b.toUInt(), type);
	case EFIND_TYPE::Byte_8:
		return SFindWhat((quint64)a.toULong(), (quint64)b.toULong(), type);
	case EFIND_TYPE::Float:
		return SFindWhat(a.toFloat(), b.toFloat(), type);
	case EFIND_TYPE::Double:
		return SFindWhat(a.toDouble(), b.toDouble(), type);
	case EFIND_TYPE::String:
		return SFindWhat(a, type);
	}

	return SFindWhat();
}
