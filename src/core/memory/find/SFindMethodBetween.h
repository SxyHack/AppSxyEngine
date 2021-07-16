#pragma once

#include "SFindMethod.h"

class SFindMethodBetween : public SFindMethod
{
	Q_OBJECT

public:
	SFindMethodBetween();
	~SFindMethodBetween();

	bool Match(char* pContent, const SFindWhat& what) override;
	bool Match(const SMemoryBuffer& buff, const SFindWhat& what) override;
};
