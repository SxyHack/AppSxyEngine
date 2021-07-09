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
