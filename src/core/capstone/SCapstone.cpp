#include "SCapstone.h"

SCapstone::SCapstone(cs_arch arch, cs_mode mode)
	: QObject()
	, _Opened(false)
	, _Insn(nullptr)
{
	_Error = cs_open(arch, mode, &_Handle);
}

SCapstone::~SCapstone()
{
}
