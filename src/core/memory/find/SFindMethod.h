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

	virtual bool Match(const QByteArray& bytes, const SFindWhat& what) = 0;
	virtual bool Match(char* pBuffer, const SFindWhat& what) = 0;

	static SFindMethod* Create(EFIND_METHOD method);

	static QVariant ToQVariant(char* data, const SFindWhat& what);
};
