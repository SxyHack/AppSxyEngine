#include "SMemoryAddress.h"
#include "SProcess.h"

SMemoryAddress::SMemoryAddress(const SMemoryBuffer& buff)
	: QThread()
	, _Buffer(buff)
	, _Locking(false)
	, _Pointer(false)
	, _Offset(0)
{
}

SMemoryAddress::SMemoryAddress(SMemoryAddress* pAddress)
	: QThread()
	, _Buffer(pAddress->_Buffer)
	, _Locking(pAddress->_Locking)
	, _Pointer(pAddress->_Pointer)
	, _Offset(pAddress->_Offset)
	, _Description(pAddress->_Description)
{

}

SMemoryAddress::~SMemoryAddress()
{
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

QString SMemoryAddress::GetTypeFormatString()
{
	return _Buffer.GetTypeFormatString();
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

void SMemoryAddress::SetDescription(const QString& text)
{
	_Description = text;
}

void SMemoryAddress::SetPointer(bool bPointer)
{
	_Pointer = bPointer;
}

bool SMemoryAddress::Read(qint32 nOffset)
{
	_Buffer.Offset = nOffset;
	return _Buffer.Update();

	//auto nReaded = (SIZE_T)0;
	//auto nAddress = _Buffer.Address + _Offset;
	//auto pProcess = _Buffer.GetProcess();
	//auto& what = _Buffer.GetWhat();
	//if (pProcess)
	//{
	//	char* pBuff = new char(what.Size);
	//	if (ReadProcessMemory(pProcess->GetHandle(), (LPVOID)nAddress, pBuff, what.Size, &nReaded))
	//	{
	//		delete pBuff;
	//		return true;
	//	}
	//}

	//return false;
}

void SMemoryAddress::run()
{

}
