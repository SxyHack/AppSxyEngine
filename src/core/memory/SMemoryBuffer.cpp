#include "SMemoryBuffer.h"
#include "core\module\SModule.h"
#include "SFindWhat.h"
#include "SFindMethod.h"
#include "SProcess.h"


SMemoryBuffer::SMemoryBuffer()
	: QObject(nullptr)
	, _Module(nullptr)
	, _What(nullptr)
	, _Process(nullptr)
	, Address(0)
{
}

SMemoryBuffer::SMemoryBuffer(quint64 nAddr, char* pBuffer, SFindWhat* pWhat, SModule* pModule, SProcess* pProcess)
	: QObject(nullptr)
	, _Module(pModule)
	, _What(pWhat)
	, _Process(pProcess)
	, Address(nAddr)
{
	Content = SFindMethod::ToQVariant(pBuffer, *pWhat);
}

SMemoryBuffer::SMemoryBuffer(const SMemoryBuffer& src)
	: QObject(nullptr)
{
	_Module = src._Module;
	_What = src._What;
	_Process = src._Process;
	Address = src.Address;
	Content = src.Content;
}



SMemoryBuffer::~SMemoryBuffer()
{
}

SMemoryBuffer& SMemoryBuffer::operator=(const SMemoryBuffer& src)
{
	_What = src._What;
	_Process = src._Process;
	_Module = src._Module;
	Address = src.Address;
	Content = src.Content;
	return *this;
}

QString SMemoryBuffer::ToString() const
{
	return Content.toString();
}

bool SMemoryBuffer::Update()
{
	if (_What == nullptr || _Process == nullptr)
	{
		qFatal("ÖÂÃü´íÎó");
		return false;
	}

	auto nReadedSize = (SIZE_T)0;
	auto pBuffer = new char[_What->Size];

	if (ReadProcessMemory(_Process->GetHandle(), (LPVOID)Address, pBuffer, _What->Size, &nReadedSize))
	{
		Content = SFindMethod::ToQVariant(pBuffer, *_What);
	}
	else
	{
		Content.clear();
	}

	delete[] pBuffer;
	return true;
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

//
//long SMemoryBuffer::GetBufferLength(EFIND_TYPE value)
//{
//	switch (value)
//	{
//	case EFIND_TYPE::Byte: return 1;
//	case EFIND_TYPE::Byte_2: return 2;
//	case EFIND_TYPE::Byte_4: return 4;
//	case EFIND_TYPE::Byte_8: return 8;
//	case EFIND_TYPE::Float:  return sizeof(FLOAT);
//	case EFIND_TYPE::Double: return sizeof(DOUBLE);
//	case EFIND_TYPE::String: return 1;
//	case EFIND_TYPE::All:    return 1;
//	}
//
//	return 0;
//}
