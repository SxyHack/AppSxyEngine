#pragma once

#include "SCapstone.h"

class SDisassemblerX86 : public SCapstone
{
	Q_OBJECT

public:
	SDisassemblerX86();
	~SDisassemblerX86();

	bool Disasm(quint8* pCode, quint32 length, quint64 nAddress);
};
