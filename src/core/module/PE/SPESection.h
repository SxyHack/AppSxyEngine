#pragma once

#include <QObject>
#include "global.h"

class SPESection : public QObject
{
	Q_OBJECT

public:
	SPESection();
	SPESection(const PIMAGE_SECTION_HEADER pHeader, quint64 imageBase);
	SPESection(const SPESection& src);
	~SPESection();

	quint64 GetFixedSize();

public:
	PIMAGE_SECTION_HEADER Header;
	QString Name;
	quint64 RVA;  // 相对虚拟地址
	quint64 FOA;  // 文件偏移地址
	quint64 ImageAddress;  // 载入到内存后的地址, ImageBase + RVA;

	//这个成员在一个共用体中，这个共用体中还有另外一个成员，由于用处不大我们就不讲解了，
	//主要讲解VirtualSize的含义。这个成员指定了该节区装入内存后的总大小，以字节为单位，
	//如果此值大于SizeOfRawData的值，那么大出的部分将用0x00填充。
	//这个成员只对可执行文件有效，如果是obj文件此成员的值为0
	quint64 ImageSize;

	// 指出零该节区在硬盘文件中的地址，这个数值是从文件头开始算起的偏移量，也就是说这个地址是一个文件偏移地址(FOA)。
	// 它的值必须是FileAlignment的整数倍。如果这个部分仅包含未初始化的数据，则将此成员设置为零
	quint64 RawAddress;

	// 指定了该节区在硬盘上初始化数据的大小，以字节为单位。
	// 它的值必须是FileAlignment的整数倍，如果小于Misc.VirtualSize，
	// 那么该部分的其余部分将用0x00填充。如果该部分仅包含未初始化的数据，那么这个值将会为零
	quint64 RawSize;
};

// typedef 
typedef QList<SPESection> LIST_PESECTION;
