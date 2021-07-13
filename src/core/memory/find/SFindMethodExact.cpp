#include "SFindMethodExact.h"

SFindMethodExact::SFindMethodExact()
	: SFindMethod()
{
}

SFindMethodExact::~SFindMethodExact()
{
}

bool SFindMethodExact::Match(const QByteArray& bytes, const SFindWhat& what)
{
	//qDebug("WhatType:%d %s", what.A.type(), what.A.typeName());
	return false;
}

bool SFindMethodExact::Match(char* pBuffer, const SFindWhat& what)
{
	auto v = ToQVariant(pBuffer, what);
	//auto a = what.A.toUInt();
	//auto b = v.toUInt();
	//return a == b;
	return v == what.A;
}
