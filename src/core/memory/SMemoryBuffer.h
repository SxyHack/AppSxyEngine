#pragma once

#include <QObject>
#include <QMap>
#include <QVariant>

#include "memory_define.h"

class SModule;
class SFindWhat;
class SProcess;
//
// �ڴ����ҵ����ڴ�顣
// NOTE: sizeof()�Ĵ�С���ܹ�����Щ�ط��Ǹ�ֵ������
// ������󣬻�����������⣬����Ҫ�ܶ��ֶ�ʱ��Ҫ��ֵ����
// �޸ĳ�ָ��
//
class SMemoryBuffer : public QObject
{
	Q_OBJECT

public:
	SMemoryBuffer();
	SMemoryBuffer(quint64 nAddr, char* pContent, SFindWhat* pWhat, SModule* pModule, SProcess* pProcess);
	SMemoryBuffer(const SMemoryBuffer& src);
	~SMemoryBuffer();

	SMemoryBuffer& operator=(const SMemoryBuffer& src);
	//
	// ���¶�ȡ�ڴ���ֵ
	//
	bool Update();

	EFIND_TYPE GetType();
	SFindWhat& GetWhat() const;
	SProcess* GetProcess();
	QString GetTypeFormatString() const;
	QString ToString() const;
	QString ToHex() const;
	
	//
	// ת����1�ֽ�
	//
	quint8 ToByte();

	//
	// ת�������֣�����Integer, Float, Double
	//
	template<class T>
	bool ToNumber(T& ret);

public:
	static bool IsCanonicalAddress(quint64 address);

public:
	quint64  Address;
	char*    Bytes;
	qint32   Offset;

	//QVariant Content;

protected:
	SProcess*  _Process;
	SModule*   _Module;
	SFindWhat* _What;
};


typedef QMap<quint64, SMemoryBuffer> MEMORY_BUFF_MAP;
typedef QList<SMemoryBuffer>         MEMORY_BUFF_LIST;

Q_DECLARE_METATYPE(SMemoryBuffer);

template<class T>
bool SMemoryBuffer::ToNumber(T& ret)
{
	const int SIZE = sizeof(T);

	if (_What->Size >= SIZE)
	{
		auto qByteArray = QByteArray(Bytes, _What->Size);
		auto nInt = qByteArray.toUInt();
		ret = nInt;
		ret = *(T*)Bytes;
		return true;
	}

	Q_ASSERT(_What);
	Q_ASSERT(_Process);
	auto nReadedSize = (SIZE_T)0;
	auto pBuffer = new char[SIZE];

	if (ReadProcessMemory(_Process->GetHandle(), (LPVOID)Address, pBuffer, SIZE, &nReadedSize))
	{
		ret = *(T*)pBuffer;
		delete pBuffer;
		return true;
	}

	delete pBuffer;
	return false;
}

