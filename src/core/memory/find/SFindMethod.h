#pragma once

#include <QObject>
#include "SFindWhat.h"
//
// ‘ı√¥’“
//
class SFindMethod : public QObject
{
	Q_OBJECT

public:
	SFindMethod();
	~SFindMethod();

	virtual bool Match(char* pContent, const SFindWhat& what) = 0;
	virtual bool Match(const SMemoryBuffer& buff, const SFindWhat& what) = 0;

public:
	static SFindMethod* Create(EFIND_METHOD method);
	static QVariant ToQVariant(char* pData, const SFindWhat& what);
};
