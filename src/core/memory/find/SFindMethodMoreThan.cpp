#include "SFindMethodMoreThan.h"

SFindMethodMoreThan::SFindMethodMoreThan()
	: SFindMethod()
{
}

SFindMethodMoreThan::~SFindMethodMoreThan()
{
}

bool SFindMethodMoreThan::Match(char* pContent, const SFindWhat& what)
{
	auto v = ToQVariant(pContent, what);
	return v >= what.A;
}

bool SFindMethodMoreThan::Match(const SMemoryBuffer& buff, const SFindWhat& what)
{
	return buff.Content >= what.A;
}
