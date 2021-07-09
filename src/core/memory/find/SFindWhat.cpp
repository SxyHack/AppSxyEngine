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
