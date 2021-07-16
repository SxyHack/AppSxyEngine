#include "SMemoryAddressManager.h"

SMemoryAddressManager::SMemoryAddressManager()
	: QObject(nullptr)
{
}

SMemoryAddressManager::~SMemoryAddressManager()
{
}

void SMemoryAddressManager::AppendAddress(const SMemoryBuffer& buff)
{
	_List.append(buff);
}

void SMemoryAddressManager::RemoveAddress(int i)
{
	_List.removeAt(i);
}
