#pragma once

#include "SFindMethod.h"

class SFindMethodLessThan : public SFindMethod
{
	Q_OBJECT

public:
	SFindMethodLessThan();
	~SFindMethodLessThan();

	bool Match(char* pContent, const SFindWhat& what) override;
	bool Match(const SMemoryBuffer& buff, const SFindWhat& what) override;
};
