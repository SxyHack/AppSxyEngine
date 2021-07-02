#include "SMemoryRegion.h"

SMemoryRegion::SMemoryRegion(const MEMORY_BASIC_INFORMATION& mbi, SModule* pModule)
	: QObject(nullptr)
	, _Module(pModule)
{
	ZeroMemory(&Content, sizeof(MEMORY_BASIC_INFORMATION));
	CopyMemory(&Content, &mbi, sizeof(mbi));
}

SMemoryRegion::SMemoryRegion(const SMemoryRegion& src)
{
	_Module = src._Module;
	Desc = src.Desc;
	CopyMemory(&Content, &src.Content, sizeof(MEMORY_BASIC_INFORMATION));
}

SMemoryRegion::~SMemoryRegion()
{
}
