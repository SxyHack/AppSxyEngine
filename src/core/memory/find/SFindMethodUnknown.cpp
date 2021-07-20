#include "SFindMethodUnknown.h"

SFindMethodUnknown::SFindMethodUnknown()
	: SFindMethod()
{
}

SFindMethodUnknown::~SFindMethodUnknown()
{
}

bool SFindMethodUnknown::Match(char* pContent, const SFindWhat& what)
{
	auto v = ToQVariant(pContent, what);
	return v != 0;
}

bool SFindMethodUnknown::Match(const SMemoryBuffer& buff, const SFindWhat& what)
{
	auto v = ToQVariant(buff.Bytes, buff.GetWhat());
	return v != 0;
}
