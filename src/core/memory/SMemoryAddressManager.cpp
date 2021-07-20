#include "SMemoryAddressManager.h"

SMemoryAddressManager::SMemoryAddressManager()
	: QObject(nullptr)
{
}

SMemoryAddressManager::~SMemoryAddressManager()
{
}

SMemoryAddress* SMemoryAddressManager::AppendAddress(const SMemoryBuffer& buff)
{
	auto pAddress = new SMemoryAddress(buff);
	_List.append(pAddress);
	return pAddress;
}

void SMemoryAddressManager::RemoveAddress(int i)
{
	delete _List.at(i);
	_List.removeAt(i);
}

void SMemoryAddressManager::RemoveAll()
{
	for (auto pItem : _List)
		delete pItem;

	_List.clear();
}
