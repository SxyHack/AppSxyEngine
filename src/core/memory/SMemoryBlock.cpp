#include "SMemoryBlock.h"
#include "core\module\SModule.h"

SMemoryBlock::SMemoryBlock(SModule* pMod)
	: QObject(nullptr)
	, _Module(pMod)
	, Address(0)
	, Size(0)
{
}

SMemoryBlock::~SMemoryBlock()
{
}

bool SMemoryBlock::IsCanonicalAddress(quint64 address)
{
#ifndef _WIN64
	// 32-bit mode only supports 4GB max, so limits are
	// not an issue
	return true;
#else
	// The most-significant 16 bits must be all 1 or all 0.
	// (64 - 16) = 48bit linear address range.
	//
	// 0xFFFF800000000000 = Significant 16 bits set
	// 0x0000800000000000 = 48th bit set
	return (((address & 0xFFFF800000000000) + 0x800000000000) & ~0x800000000000) == 0;
#endif //_WIN64
}
