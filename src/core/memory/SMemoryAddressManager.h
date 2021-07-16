#pragma once

#include <QObject>
#include "SMemoryBuffer.h"

class SMemoryAddressManager : public QObject
{
	Q_OBJECT

public:
	SMemoryAddressManager();
	~SMemoryAddressManager();

	void AppendAddress(const SMemoryBuffer& buff);
	void RemoveAddress(int i);

protected:
	MEMORY_BUFF_LIST _List;
};
