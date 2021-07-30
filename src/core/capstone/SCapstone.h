#pragma once

#include <QObject>

#include "capstone.h"

class SCapstone : public QObject
{
	Q_OBJECT

public:
	SCapstone(cs_arch arch, cs_mode mode);
	~SCapstone();

protected:
	bool     _Opened;
	csh      _Handle;
	cs_insn* _Insn;
	cs_err   _Error;
};
