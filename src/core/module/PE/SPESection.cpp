#include "SPESection.h"
#include "module_types.h"

SPESection::SPESection()
	: QObject(nullptr)
	, Header(NULL)
{
	FOA = 0;
	RVA = 0;
	ImageAddress = 0;
	ImageSize = 0;
	RawAddress = 0;
	RawSize = 0;
}

SPESection::SPESection(const PIMAGE_SECTION_HEADER pHeader, quint64 imageBase) : QObject(nullptr)
, Header(pHeader)
{
	RVA = pHeader->VirtualAddress;
	FOA = pHeader->PointerToRawData;
	ImageAddress = imageBase + RVA;
	ImageSize = pHeader->Misc.VirtualSize;
	RawAddress = pHeader->PointerToRawData;
	RawSize = pHeader->SizeOfRawData;
	Name = QString::fromLocal8Bit((char*)pHeader->Name, IMAGE_SIZEOF_SHORT_NAME);
}

SPESection::SPESection(const SPESection& src)
{
	Header = src.Header;
	RVA = src.RVA;
	FOA = src.FOA;
	ImageAddress = src.ImageAddress;
	ImageSize = src.ImageSize;
	RawAddress = src.RawAddress;
	RawSize = src.RawSize;
	Name = src.Name;
}

SPESection::~SPESection()
{
}

quint64 SPESection::GetFixedSize()
{
	quint64 nFixedSize = ImageSize;
	if (nFixedSize % REGION_ALIGN_SIZE)
	{
		nFixedSize += REGION_ALIGN_SIZE - (ImageSize % REGION_ALIGN_SIZE);
	}

	return nFixedSize;
}
