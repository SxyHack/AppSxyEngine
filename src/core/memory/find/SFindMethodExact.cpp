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
	//auto a = what.A.toUInt();
	//auto b = v.toUInt();
	//return a == b;
	return v == what.A;
}

bool SFindMethodExact::Match(const SMemoryBuffer& buff, const SFindWhat& what)
{
	return buff.Content == what.A;
}
