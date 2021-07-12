#include "SFindWhat.h"


SFindWhat::SFindWhat(quint8 v)
	: A(v)
	, B(v)
	, Size(sizeof(quint8))
{
}

SFindWhat::SFindWhat(quint8 a, quint8 b)
	: A(a)
	, B(b)
	, Size(sizeof(quint8))
{
}

SFindWhat::SFindWhat(quint16 v)
	: A(v)
	, B(v)
	, Size(sizeof(quint16))
{
}

SFindWhat::SFindWhat(quint16 a, quint16 b)
	: A(a)
	, B(b)
	, Size(sizeof(quint16))
{
}

SFindWhat::SFindWhat(quint32 v)
	: A(v)
	, B(v)
	, Size(sizeof(quint32))
{
}

SFindWhat::SFindWhat(quint32 a, quint32 b)
	: A(a)
	, B(b)
	, Size(sizeof(quint32))
{
}

SFindWhat::SFindWhat(quint64 v)
	: A(v)
	, B(v)
	, Size(sizeof(quint64))
{
}

SFindWhat::SFindWhat(quint64 a, quint64 b)
	: A(a)
	, B(b)
	, Size(sizeof(quint64))
{

}

SFindWhat::SFindWhat(float v)
	: A(v)
	, B(v)
	, Size(sizeof(float))
{

}

SFindWhat::SFindWhat(float a, float b)
	: A(a)
	, B(b)
	, Size(sizeof(float))
{

}

SFindWhat::SFindWhat(double v)
	: A(v)
	, B(v)
	, Size(sizeof(double))
{

}

SFindWhat::SFindWhat(double a, double b)
	: A(a)
	, B(b)
	, Size(sizeof(double))
{

}

SFindWhat::SFindWhat(QString txt)
	: A(txt)
	, Size(txt.length())
{
}

SFindWhat::SFindWhat(const SFindWhat& src)
{
	A = src.A;
	B = src.B;
	Size = src.Size;
}

SFindWhat::SFindWhat()
{
	Size = 0;
}

SFindWhat::~SFindWhat()
{
}

void SFindWhat::AppendBuff(const SMemoryBuffer& buff)
{
	_FoundAddressList.append(buff.Address);
	_FoundBuffMap.insert(buff.Address, buff);
}

long SFindWhat::GetFoundCount()
{
	return _FoundAddressList.count();
}

SFindWhat SFindWhat::Create(const QString& a, const QString& b, EFIND_TYPE type)
{
	switch (type)
	{
	case EFIND_TYPE::Byte:
		return SFindWhat((quint8)a.toUInt(), (quint8)b.toUInt());
	case EFIND_TYPE::Byte_2:
		return SFindWhat((quint16)a.toUInt(), (quint16)b.toUInt());
	case EFIND_TYPE::Byte_4:
		return SFindWhat((quint32)a.toUInt(), (quint32)b.toUInt());
	case EFIND_TYPE::Byte_8:
		return SFindWhat((quint64)a.toULong(), (quint64)b.toULong());
	case EFIND_TYPE::Float:
		return SFindWhat(a.toFloat(), b.toFloat());
	case EFIND_TYPE::Double:
		return SFindWhat(a.toDouble(), b.toDouble());
	case EFIND_TYPE::String:
		return SFindWhat(a);
	case EFIND_TYPE::All:
		break;
	}

	return SFindWhat();
}
