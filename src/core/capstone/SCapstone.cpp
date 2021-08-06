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

bool SCapstone::Disasm(quint8* pCode, quint32 length, quint64 nAddress)
{
	if (_Error != CS_ERR_OK)
		return false;

	auto nCount = cs_disasm(_Handle, pCode, length, nAddress, 0, &_Insn);
	if (nCount > 0)
	{
		
		return true;
	}

	return false;
}
