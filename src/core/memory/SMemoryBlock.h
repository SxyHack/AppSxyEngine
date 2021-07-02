#pragma once

#include <QObject>
#include <QWeakPointer>
#include <QSharedPointer>

//
// memory block type
// �ڴ��ֵ������, ���ַ�����N�ֽ������ȵ� 
enum class MBT
{
	Byte,
	Byte_2,
	Byte_4,
	Byte_8,
	Float,
	Double,
	String
};


class SModule;

class SMemoryBlock : public QObject
{
	Q_OBJECT

public:
	SMemoryBlock(SModule* pModule);
	~SMemoryBlock();

	static bool IsCanonicalAddress(quint64 address);

public:
	quint64 Address;
	quint64 Size;
	MBT     Type;
	
protected:
	SModule* _Module;
};
