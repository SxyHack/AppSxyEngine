#include "SFindMethodExact.h"

SFindMethodExact::SFindMethodExact()
	: SFindMethod()
{
}

SFindMethodExact::~SFindMethodExact()
{
}

bool SFindMethodExact::Match(char* pContent, const SFindWhat& what)
{
	auto v = ToQVariant(pContent, what);
	return v == what.A;
}

bool SFindMethodExact::Match(const SMemoryBuffer& buff, const SFindWhat& what)
{
	auto v = ToQVariant(buff.Bytes, buff.GetWhat());
	return v == what.A;
}
