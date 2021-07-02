#include "SPEParser.h"
#include "SModule.h"
#include "SMemoryBlock.h"
#include "libs\ntdll\ntdll.h"

#include <ntstatus.h>

SPEParser::SPEParser(SModule* pModule)
	: QObject(nullptr)
	, _Module(pModule)
{
	Execute();
}

SPEParser::~SPEParser()
{
}

bool SPEParser::Execute()
{
	if (!NT_SUCCESS(ParseDOSHead()))
	{
		qCritical("����PE-DOSͷʧ��");
		return false;
	}

	if (!NT_SUCCESS(ParseNTHead()))
	{
		qCritical("����PE-NTͷʧ��");
		return false;
	}

	if (!NT_SUCCESS(ParseSection()))
	{
		qCritical("����PE-����ʧ��");
		return false;
	}

	return true;
}

NTSTATUS SPEParser::ParseDOSHead()
{
	if (_Module->FileMapBase == 0)
		return STATUS_INVALID_PARAMETER;

	if (_Module->FileSize < sizeof(IMAGE_DOS_HEADER))
		return STATUS_INVALID_IMAGE_FORMAT;

	const PIMAGE_DOS_HEADER pDosHeaders = (PIMAGE_DOS_HEADER)_Module->FileMapBase;
	if (pDosHeaders->e_magic != IMAGE_DOS_SIGNATURE)
		return STATUS_INVALID_IMAGE_FORMAT;

	_Module->ImageDosHeader = pDosHeaders;

	return STATUS_SUCCESS;
}

NTSTATUS SPEParser::ParseNTHead()
{
	if (_Module->ImageDosHeader == NULL)
		return STATUS_INVALID_PARAMETER;
	const ULONG e_lfanew = _Module->ImageDosHeader->e_lfanew;
	const ULONG sizeOfPeSignature = sizeof('PE00');
	if (e_lfanew >= _Module->FileSize ||
		e_lfanew >= (ULONG_MAX - sizeOfPeSignature - sizeof(IMAGE_FILE_HEADER)) ||
		(e_lfanew + sizeOfPeSignature + sizeof(IMAGE_FILE_HEADER)) >= _Module->FileSize)
		return STATUS_INVALID_IMAGE_FORMAT;


	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)(_Module->FileMapBase + e_lfanew);
	// RtlImageNtHeaderEx verifies that the range does not cross the UM <-> KM boundary here,
	// but it would cost a syscall to query this address as it varies between OS versions 
	// TODO: or do we already have this info somewhere?
	if (!SMemoryBlock::IsCanonicalAddress((quint64)pNtHeaders + sizeof(IMAGE_NT_HEADERS)))
		return STATUS_INVALID_IMAGE_FORMAT;
	if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
		return STATUS_INVALID_IMAGE_FORMAT;

	_Module->ImageNtHeaders = pNtHeaders;
	_Module->ImageFileHeader = &pNtHeaders->FileHeader;
	_Module->ImageOptHeader = &pNtHeaders->OptionalHeader;

	quint64 OEP = HEADER_FIELD(pNtHeaders, AddressOfEntryPoint);
	BOOL bIsDLL = ((pNtHeaders->FileHeader.Characteristics & IMAGE_FILE_DLL) == IMAGE_FILE_DLL);

	_Module->EntryPoint = (bIsDLL && OEP == 0)
		? 0
		: OEP + _Module->FileMapBase;
	_Module->ImageBase = HEADER_FIELD(pNtHeaders, ImageBase);
	_Module->Machine = PE_MACHINE(pNtHeaders->FileHeader.Machine);
	_Module->NumberOfSections = pNtHeaders->FileHeader.NumberOfSections;

	return STATUS_SUCCESS;
}

NTSTATUS SPEParser::ParseSection()
{
	if (_Module->ImageNtHeaders == NULL)
	{
		qCritical("Must be call ParseNtHeader() before.");
		return STATUS_INVALID_IMAGE_FORMAT;
	}

	if (_Module->NumberOfSections <= 0)
	{
		return STATUS_INVALID_IMAGE_FORMAT;
	}

	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(_Module->ImageNtHeaders);

	_Module->SectionEntry = pSection;

	QString qsLog = "Sections:\n";

	for (int i = 0; i < _Module->NumberOfSections; i++)
	{
		SPESection section(pSection, _Module->ImageBase);

		qsLog += QString().sprintf("\t%s: Image:%p(%d) Raw:%p(%d)\n",
			section.Name.toUtf8().data(),
			section.ImageAddress,
			section.ImageSize,
			section.RawAddress,
			section.RawSize);

		_Module->Sections.append(section);
		pSection++;
	}

	qDebug(qsLog.toUtf8().data());

	return STATUS_SUCCESS;

}