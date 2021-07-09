#pragma once

#include "SFindMethod.h"

class SFindMethodExact : public SFindMethod
{
	Q_OBJECT

public:
	SFindMethodExact();
	~SFindMethodExact();

	bool Match(const QByteArray& bytes, const SFindWhat& what) override;
};
