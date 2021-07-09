#include "SMemoryBuffer.h"
#include "core\module\SModule.h"

SMemoryBuffer::SMemoryBuffer(quint64 addr, const QByteArray& bytes, SModule* pMod)
	: QObject(nullptr)
	, _Module(pMod)
	, Address(addr)
	, Content(bytes)
{
}

SMemoryBuffer::SMemoryBuffer(const SMemoryBuffer& src)
	: QObject(nullptr)
{
	_Module = src._Module;
	Address = src.Address;
	Content = src.Content;
}

SMemoryBuffer::~SMemoryBuffer()
{
}

SMemoryBuffer& SMemoryBuffer::operator=(const SMemoryBuffer& src)
{
	_Module = src._Module;
	Address = src.Address;
	Content = src.Content;
	return *this;
}

bool SMemoryBuffer::IsCanonicalAddress(quint64 address)
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

long SMemoryBuffer::GetBufferLength(EFIND_TYPE value)
{
	switch (value)
	{
	case EFIND_TYPE::Byte: return 1;
	case EFIND_TYPE::Byte_2: return 2;
	case EFIND_TYPE::Byte_4: return 4;
	case EFIND_TYPE::Byte_8: return 8;
	case EFIND_TYPE::Float:  return sizeof(FLOAT);
	case EFIND_TYPE::Double: return sizeof(DOUBLE);
	case EFIND_TYPE::String: return 1;
	case EFIND_TYPE::All:    return 1;
	}

	return 0;
}
