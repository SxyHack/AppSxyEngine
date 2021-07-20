#pragma once

#include <QObject>
#include "SMemoryAddress.h"

class SMemoryAddressManager : public QObject
{
	Q_OBJECT

public:
	SMemoryAddressManager();
	~SMemoryAddressManager();

	SMemoryAddress* AppendAddress(const SMemoryBuffer& buff);
	void RemoveAddress(int i);
	void RemoveAll();

protected:
	PADDRESS_LIST _List;
};
