#pragma once

#include <QThread>
#include "SMemoryBuffer.h"

class SMemoryAddress : public QThread
{
	Q_OBJECT

public:
	SMemoryAddress(const SMemoryBuffer& buff);
	~SMemoryAddress();

protected:
	void run() override;

private:
	SMemoryBuffer _Buffer;
	bool _Locking;
};
