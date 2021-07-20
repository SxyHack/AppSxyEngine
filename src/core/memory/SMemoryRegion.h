#pragma once

#include <QObject>
#include <QList>
#include "global.h"

class SModule;

//
// ��װͨ��VirtualQueryEx()��ѯ�����ڴ�ҳ
//
class SMemoryRegion : public QObject
{
	Q_OBJECT

public:
	SMemoryRegion(const MEMORY_BASIC_INFORMATION& mbi, SModule* pModule = nullptr);
	SMemoryRegion(const SMemoryRegion& src);
	~SMemoryRegion();

public:
	MEMORY_BASIC_INFORMATION Mbi;
	QString Desc;

private:
	SModule* _Module;
};


typedef QList<SMemoryRegion> LIST_MEMORY_REGION;