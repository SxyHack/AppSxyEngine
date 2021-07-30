#include "SDisassemblerX86.h"

SDisassemblerX86::SDisassemblerX86()
	: SCapstone(CS_ARCH_X86, CS_MODE_64)
{
}

SDisassemblerX86::~SDisassemblerX86()
{
}

bool SDisassemblerX86::Disasm(quint8* pCode, quint32 length, quint64 nAddress)
{
	auto nCount = cs_disasm(_Handle, pCode, length, nAddress, 0, &_Insn);
	if (nCount > 0)
	{

		return true;
	}

	return false;
}
