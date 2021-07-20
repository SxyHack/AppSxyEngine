#include "SMemoryRegion.h"

SMemoryRegion::SMemoryRegion(const MEMORY_BASIC_INFORMATION& mbi, SModule* pModule)
	: QObject(nullptr)
	, _Module(pModule)
{
	ZeroMemory(&Mbi, sizeof(MEMORY_BASIC_INFORMATION));
	CopyMemory(&Mbi, &mbi, sizeof(mbi));
}

SMemoryRegion::SMemoryRegion(const SMemoryRegion& src)
{
	_Module = src._Module;
	Desc = src.Desc;
	CopyMemory(&Mbi, &src.Mbi, sizeof(MEMORY_BASIC_INFORMATION));
}

SMemoryRegion::~SMemoryRegion()
{
}
