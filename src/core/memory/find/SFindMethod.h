#pragma once

#include <QObject>
#include "SFindWhat.h"
//
// ��ô��
//
class SFindMethod : public QObject
{
	Q_OBJECT

public:
	SFindMethod();
	~SFindMethod();

	virtual bool Match(const QByteArray& bytes, const SFindWhat& what) = 0;
};
