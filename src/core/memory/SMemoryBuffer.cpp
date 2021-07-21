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
	, Offset(0)
	, Bytes(nullptr)
{
}

SMemoryBuffer::SMemoryBuffer(quint64 nAddr, char* pBuffer, SFindWhat* pWhat, SModule* pModule, SProcess* pProcess)
	: QObject(nullptr)
	, _Module(pModule)
	, _What(pWhat)
	, _Process(pProcess)
	, Bytes(new char[pWhat->Size])
	, Address(nAddr)
	, Offset(0)
{
	CopyMemory(Bytes, pBuffer, pWhat->Size);
}

SMemoryBuffer::SMemoryBuffer(const SMemoryBuffer& src)
	: QObject(nullptr)
	, Bytes(new char[src._What->Size])
{
	_Module = src._Module;
	_What = src._What;
	_Process = src._Process;
	Address = src.Address;
	Offset = src.Offset;
	CopyMemory(Bytes, src.Bytes, _What->Size);
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
	Offset = src.Offset;

	if (Bytes) delete Bytes;

	Bytes = new char[src._What->Size];
	CopyMemory(Bytes, src.Bytes, _What->Size);

	return *this;
}

QString SMemoryBuffer::ToString() const
{
	return SFindMethod::ToQVariant(Bytes, *_What).toString();
}

QString SMemoryBuffer::ToHex() const
{
	auto nNumber = SFindMethod::ToQVariant(Bytes, *_What).toUInt();
	return QString::number(nNumber, 16);
}

QString SMemoryBuffer::GetTypeFormatString() const
{
	switch (_What->Type)
	{
	case EFIND_TYPE::Byte:
		return "1字节";
	case EFIND_TYPE::Byte_2:
		return "2字节";
	case EFIND_TYPE::Byte_4:
		return "4字节";
	case EFIND_TYPE::Byte_8:
		return "8字节";
	case EFIND_TYPE::Float:
		return "单浮点";
	case EFIND_TYPE::Double:
		return "双浮点";
	case EFIND_TYPE::String:
		return "字符串";
	}

	return "??";
}

quint8 SMemoryBuffer::ToByte()
{
	return *(quint8*)Bytes;
}

bool SMemoryBuffer::Update()
{
	if (_What == nullptr || _Process == nullptr)
	{
		qFatal("致命错误");
		return false;
	}

	auto nOffsetAddr = Address + Offset;
	auto nReadedSize = (SIZE_T)0;
	return ReadProcessMemory(_Process->GetHandle(), (LPVOID)nOffsetAddr, Bytes, _What->Size, &nReadedSize);
}

void SMemoryBuffer::SetType(EFIND_TYPE type)
{
	_What->Type = type;
}

EFIND_TYPE SMemoryBuffer::GetType()
{
	return _What->Type;
}

SFindWhat& SMemoryBuffer::GetWhat() const
{
	return *_What;
}

SProcess* SMemoryBuffer::GetProcess()
{
	return _Process;
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
