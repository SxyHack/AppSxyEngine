#include "SModule.h"
#include "core\extras\NtExtras.h"

#include <QFileInfo>

SModule::SModule()
	: QObject(nullptr)
	, ID(0)
	, OwnPID(0)
	, EntryPoint(0)
	, ModBase(0)
	, ModSize(0)
	, FileSize(0)
	, FileMapBase(0)
	, Handle(0)
	, ImageDosHeader(NULL)
	, ImageNtHeaders(NULL)
	, SectionEntry(NULL)
	, ImageFileHeader(NULL)
	, ImageOptHeader(NULL)
	, NumberOfSections(-1)
	, ImageBase(0)
	, Machine(PE_MACHINE::X64)
	, Party(MODULE_PARTY::USER)
{
}

SModule::SModule(const SModule& src)
	: QObject(nullptr)
{
	ID = src.ID;
	OwnPID = src.OwnPID;
	EntryPoint = src.EntryPoint;
	ModBase = src.ModBase;
	ModSize = src.ModSize;
	FileSize = src.FileSize;
	FileMapBase = src.FileMapBase;
	Handle = src.Handle;
	ImageBase = src.ImageBase;
	ImageDosHeader = src.ImageDosHeader;
	ImageNtHeaders = src.ImageNtHeaders;
	ImageFileHeader = src.ImageFileHeader;
	ImageOptHeader = src.ImageOptHeader;
	SectionEntry = src.SectionEntry;
	NumberOfSections = src.NumberOfSections;
	Machine = src.Machine;
	Party = src.Party;
}

SModule::~SModule()
{
}

SModule* SModule::Create(SProcess* pProc, const MODULEENTRY32& tlh32Entry)
{
	auto pModule = new SModule;
	pModule->FilePath = QString::fromWCharArray(tlh32Entry.szExePath);
	pModule->FileName = QString::fromWCharArray(tlh32Entry.szModule);
	pModule->ModBase = quint64(tlh32Entry.modBaseAddr);
	pModule->ModSize = tlh32Entry.modBaseSize;

	qDebug("Load: %s [%p, %p]", pModule->FileName.toUtf8().data(), pModule->ModBase, pModule->ModBase + pModule->ModSize);

	QFileInfo fiMod(pModule->FilePath);
	pModule->FileExt = fiMod.suffix();
	pModule->FileSize = fiMod.size();

	auto qsSystemDir = GetSystemDir();
	pModule->Party = pModule->FilePath.contains(qsSystemDir, Qt::CaseInsensitive)
		? MODULE_PARTY::SYSTEM
		: MODULE_PARTY::USER;

	QFile qfModule(pModule->FilePath);
	if (qfModule.open(QIODevice::ReadOnly))
	{
		pModule->FileSize = qfModule.size();
		pModule->FileMapBase = (quint64)qfModule.map(0, pModule->FileSize);
	}
	else
	{
		pModule->FileSize = 0;
		pModule->FileMapBase = 0;
	}

	return pModule;
}
