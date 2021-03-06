#pragma once

#include "SFindMethod.h"

class SFindMethodExact : public SFindMethod
{
	Q_OBJECT

public:
	SFindMethodExact();
	~SFindMethodExact();

	bool Match(char* pContent, const SFindWhat& what) override;
	bool Match(const SMemoryBuffer& buff, const SFindWhat& what) override;
};
