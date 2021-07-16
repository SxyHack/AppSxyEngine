#include "SFindMethodBetween.h"

SFindMethodBetween::SFindMethodBetween()
	: SFindMethod()
{
}

SFindMethodBetween::~SFindMethodBetween()
{
}

bool SFindMethodBetween::Match(char* pContent, const SFindWhat& what)
{
	auto v = ToQVariant(pContent, what);
	return what.A <= v && v <= what.B;
}

bool SFindMethodBetween::Match(const SMemoryBuffer& buff, const SFindWhat& what)
{
	return what.A <= buff.Content && buff.Content <= what.B;
}
