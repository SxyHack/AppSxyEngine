#pragma once

#include <QObject>

#include "capstone.h"

class SCapstone : public QObject
{
	Q_OBJECT

public:
	SCapstone(cs_arch arch, cs_mode mode);
	~SCapstone();

	//
	// ��16��������ת���ɷ��������
	// 
	bool Disasm(quint8* pCode, quint32 length, quint64 nAddress);

protected:
	bool     _Opened;
	csh      _Handle;
	cs_insn* _Insn;
	cs_err   _Error;
};
