#pragma once

#include <QThread>
#include "SMemoryBuffer.h"

class SMemoryAddress : public QThread
{
	Q_OBJECT

public:
	SMemoryAddress(const SMemoryBuffer& buff);
	SMemoryAddress(SMemoryAddress* pAddress);
	~SMemoryAddress();

	EFIND_TYPE GetType();
	quint64 GetAddress();
	QString GetAddressHex();
	QString GetTypeFormatString();
	//
	// 将地址指向的内存数值转换成字符串
	//
	QString ToString();
	//
	// 将地址指向的内存数值转换成HEX
	// 
	QString ToHex();

	quint8 ToByte();

	void SetDescription(const QString& text);
	void SetPointer(bool bPointer);
	bool Read(qint32 nOffset);
	//
	// 转换成数字，包括Integer, Float, Double
	//
	template<class T>
	bool ToNumber(T& ret);

protected:
	void run() override;

private:
	SMemoryBuffer  _Buffer;
	bool           _Locking;
	bool           _Pointer;
	QString        _Description;
};

typedef QList<SMemoryAddress*> PADDRESS_LIST;

template<class T>
bool SMemoryAddress::ToNumber(T& ret)
{
	return _Buffer.ToNumber<T>(ret);
}
