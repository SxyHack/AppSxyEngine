#pragma once

#include "SFindMethod.h"

class SFindMethodMoreThan : public SFindMethod
{
	Q_OBJECT

public:
	SFindMethodMoreThan();
	~SFindMethodMoreThan();

	bool Match(char* pContent, const SFindWhat& what) override;
	bool Match(const SMemoryBuffer& buff, const SFindWhat& what) override;
};
