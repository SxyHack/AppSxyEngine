#pragma once

#include "SFindMethod.h"

class SFindMethodUnknown : public SFindMethod
{
	Q_OBJECT

public:
	SFindMethodUnknown();
	~SFindMethodUnknown();

	bool Match(char* pContent, const SFindWhat& what) override;
	bool Match(const SMemoryBuffer& buff, const SFindWhat& what) override;
};
