#include "SFindMethodLessThan.h"

SFindMethodLessThan::SFindMethodLessThan()
	: SFindMethod()
{
}

SFindMethodLessThan::~SFindMethodLessThan()
{
}

bool SFindMethodLessThan::Match(char* pContent, const SFindWhat& what)
{
	auto v = ToQVariant(pContent, what);
	return v <= what.A;
}

bool SFindMethodLessThan::Match(const SMemoryBuffer& buff, const SFindWhat& what)
{
	auto v = ToQVariant(buff.Bytes, buff.GetWhat());
	return v <= what.A;
}
