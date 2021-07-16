#include "SMemoryAddress.h"

SMemoryAddress::SMemoryAddress(const SMemoryBuffer& buff)
	: QThread()
	, _Buffer(buff)
	, _Locking(false)
{
}

SMemoryAddress::~SMemoryAddress()
{
}

void SMemoryAddress::run()
{

}
