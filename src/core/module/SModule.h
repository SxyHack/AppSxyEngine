#pragma once

#include <QObject>
#include <QMap>
#include <map>

#include "module_types.h"
#include "SPESection.h"

class SProcess;


class SModule : public QObject
{
	Q_OBJECT

public:
	friend class SPEParser;

	SModule();
	SModule(const SModule& src);
	~SModule();

	bool IsCodeRegion(quint64 address);

	static SModule* Create(SProcess* pProc, const MODULEENTRY32& tlh32Entry);

public:
	quint32           ID;         // ģ��ID
	quint64           OwnPID;     // ����ID
	HMODULE           Handle;     // ģ����
	quint64           ModBase;    // Module base
	quint64           ModSize;    // Module size
	quint64           EntryPoint; // Entry point
	QString           FilePath;   // ����·��, �����ļ����ֺ���չ��, ����: c:\windows\ntdll.dll
	QString           FileName;   // �ļ���, ����: ntdll.dll
	QString           FileExt;    // �ļ���չ��, ����: dll
	quint64           FileSize;   // �ļ���С
	quint64           FileMapBase;
	quint64           ImageBase;
	MODULE_PARTY      Party;

protected:
	// PE headers
	PIMAGE_DOS_HEADER      ImageDosHeader;   // Dos Header
	PIMAGE_NT_HEADERS      ImageNtHeaders;   // NT Headers
	PIMAGE_FILE_HEADER     ImageFileHeader;
	PIMAGE_OPTIONAL_HEADER ImageOptHeader;
	PIMAGE_SECTION_HEADER  SectionEntry;
	PE_MACHINE             Machine;

	// Sections
	qint32                 NumberOfSections;
	LIST_PESECTION         Sections;
};


Q_DECLARE_METATYPE(SModule);

//typedef QMap<QRange, SModule*> RANGE_MAP_MOUDLE;
typedef std::map<QRange, SModule*, QRangeCompare> RANGE_MAP_MOUDLE;
typedef QMap<QString, SModule*> NAME_MAP_MODULE;
