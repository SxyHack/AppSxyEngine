#pragma once

#include <QObject>
#include <QWeakPointer>
#include <QSharedPointer>

class SModule;

class SMemoryBlock : public QObject
{
	Q_OBJECT

public:
	SMemoryBlock(SModule* pMod);
	~SMemoryBlock();

	static bool IsCanonicalAddress(quint64 address);

public:
	quint64 Address;
	quint64 Size;
	
protected:
	SModule* _Module;
};
