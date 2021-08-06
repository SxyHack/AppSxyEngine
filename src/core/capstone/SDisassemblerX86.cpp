#include "SDisassemblerX86.h"

SDisassemblerX86::SDisassemblerX86()
	: SCapstone(CS_ARCH_X86, CS_MODE_64)
{
}

SDisassemblerX86::~SDisassemblerX86()
{
}

