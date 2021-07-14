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

QVariant SFindMethod::ToQVariant(char* pData, const SFindWhat& what)
{
	switch (what.Type)
	{
	case EFIND_TYPE::Byte:
		return QVariant(*(quint8*)pData);
	case EFIND_TYPE::Byte_2:
		return QVariant(*(quint16*)pData);
	case EFIND_TYPE::Byte_4:
		return QVariant(*(quint32*)pData);
	case EFIND_TYPE::Byte_8:
		return QVariant(*(quint64*)pData);
	case EFIND_TYPE::Float:
		return QVariant(*(float*)pData);
	case EFIND_TYPE::Double:
		return QVariant(*(double*)pData);
	case EFIND_TYPE::String:
		return QVariant(QString::fromLocal8Bit(pData, what.Size));
	}

	return QVariant();
}
