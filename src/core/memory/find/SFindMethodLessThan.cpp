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
	return buff.Content <= what.A;
}
