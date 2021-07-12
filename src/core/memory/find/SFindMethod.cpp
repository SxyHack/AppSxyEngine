#include "SFindMethod.h"
#include "SFindMethodExact.h"

SFindMethod::SFindMethod()
	: QObject(nullptr)
{
}

SFindMethod::~SFindMethod()
{
}

SFindMethod* SFindMethod::Create(EFIND_METHOD method)
{
	switch (method)
	{
	case EFIND_METHOD::Exact:
		return new SFindMethodExact();
	case EFIND_METHOD::MoreThan:
		break;
	case EFIND_METHOD::LessThan:
		break;
	case EFIND_METHOD::Between:
		break;
	case EFIND_METHOD::Unknown:
		break;
	case EFIND_METHOD::Bigger:
		break;
	case EFIND_METHOD::Smaller:
		break;
	case EFIND_METHOD::IncreaseN:
		break;
	case EFIND_METHOD::DecreaseN:
		break;
	case EFIND_METHOD::Changed:
		break;
	case EFIND_METHOD::Unchanged:
		break;
	case EFIND_METHOD::EqualBase:
		break;
	}

	return nullptr;
}
