#include "SMemoryAddress.h"
#include "SProcess.h"

SMemoryAddress::SMemoryAddress(const SMemoryBuffer& buff)
	: QThread()
	, _Buffer(buff)
	, _Locking(false)
	, _Pointer(false)
	, _Prev(nullptr)
	, _Next(nullptr)
	, _Valid(true)
{
}

SMemoryAddress::SMemoryAddress(SMemoryAddress* pAddress)
	: QThread()
	, _Buffer(pAddress->_Buffer)
	, _Locking(pAddress->_Locking)
	, _Pointer(pAddress->_Pointer)
	, _Description(pAddress->_Description)
	, _Prev(pAddress)
	, _Next(nullptr)
{
	_Valid = pAddress->ToNumber<quint64>(_Buffer.Address);
}

SMemoryAddress::~SMemoryAddress()
{
	if (_Prev)
		_Prev->SetNext(_Next);
}

EFIND_TYPE SMemoryAddress::GetType()
{
	return _Buffer.GetType();
}

quint64 SMemoryAddress::GetAddress()
{
	return _Buffer.Address;
}

QString SMemoryAddress::GetAddressHex()
{
	return QString::number(_Buffer.Address, 16).toUpper();
}

QString SMemoryAddress::GetOffsetHex()
{
	return QString::number(_Buffer.Offset, 16).toUpper();
}

QString SMemoryAddress::GetTypeFormatString()
{
	return _Buffer.GetTypeFormatString();
}

QString SMemoryAddress::GetDescription()
{
	return _Description;
}

QString SMemoryAddress::ToString()
{
	return _Buffer.ToString();
}

QString SMemoryAddress::ToHex()
{
	return _Buffer.ToHex().toUpper();
}

quint8 SMemoryAddress::ToByte()
{
	return _Buffer.ToByte();
}

SMemoryAddress* SMemoryAddress::GetLast()
{
	auto pCurrent = this;
	while (pCurrent)
	{
		auto pNext = pCurrent->GetNext();
		if (pNext == nullptr)
			return pCurrent;

		pCurrent = pNext;
	}

	return nullptr;
}

SMemoryAddress* SMemoryAddress::GetNext()
{
	return _Next;
}

SMemoryAddress* SMemoryAddress::GetPrev()
{
	return _Prev;
}

void SMemoryAddress::SetPrev(SMemoryAddress* pAddress)
{
	_Prev = pAddress;
}

void SMemoryAddress::SetNext(SMemoryAddress* pAddress)
{
	_Next = pAddress;
}

void SMemoryAddress::SetDescription(const QString& sDesc)
{
	_Description = sDesc;
}

void SMemoryAddress::SetPointer(bool bPointer)
{
	_Pointer = bPointer;

	if (!bPointer)
	{
		RemoveOffsets();
	}
}

void SMemoryAddress::SetNewAddress(quint64 nAddress)
{
	_Buffer.Address = nAddress;
}

void SMemoryAddress::SetType(EFIND_TYPE type)
{
	_Buffer.SetType(type);
}

void SMemoryAddress::RemoveOffsets()
{
	if (GetPrev() == nullptr && GetNext() == nullptr)
		return;

	auto pNext = GetNext();
	while (pNext)
	{
		auto pTmp = pNext;
		pNext = pNext->GetNext();
		delete pTmp;
	}

	_Prev = nullptr;
	_Next = nullptr;
}

bool SMemoryAddress::Read(qint32 nOffset)
{
	_Buffer.Offset = nOffset;
	return _Buffer.Update();
}

bool SMemoryAddress::IsValid()
{
	return _Valid;
}

void SMemoryAddress::run()
{

}
