#pragma once
#pragma once

#include <QPair>
#include <QString>

#include "global.h"

#define MAX_MODULE_SIZE 1024

// Macros to safely access IMAGE_NT_HEADERS fields since the compile-time typedef of this struct may not match the actual file bitness.
// Never access OptionalHeader.xx values directly unless they have the same size and offset on 32 and 64 bit. 
// IMAGE_FILE_HEADER fields are safe to use
#define IMAGE32(NtHeaders) ((NtHeaders) != nullptr && (NtHeaders)->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
#define IMAGE64(NtHeaders) ((NtHeaders) != nullptr && (NtHeaders)->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
#define HEADER_FIELD(NtHeaders, Field) (IMAGE64(NtHeaders) \
    ? ((PIMAGE_NT_HEADERS64)(NtHeaders))->OptionalHeader.Field : (IMAGE32(NtHeaders) \
        ? ((PIMAGE_NT_HEADERS32)(NtHeaders))->OptionalHeader.Field \
        : 0))
#define THUNK_VAL(NtHeaders, Ptr, Val) (IMAGE64(NtHeaders) \
    ? ((PIMAGE_THUNK_DATA64)(Ptr))->Val : (IMAGE32(NtHeaders) \
        ? ((PIMAGE_THUNK_DATA32)(Ptr))->Val \
        : 0))


enum class PE_MACHINE
{
	X86 = 0x014c,
	X64 = 0x8664,
	INTEL_ITANIUM = 0x0200
};

// 参考 https://bbs.pediy.com/thread-252795.htm#msg_header_h1_1
enum class PE_DATA_DIRECTORY
{
	EXPORT_TABLE = 0x00,
	IMPORT_TABLE = 0x01,
	IMPORT_BOUND_TABLE = 0x0B,
	IMPORT_ADDRESS_TABLE = 0x0C,
	IMPORT_DELAY_DESC = 0x0D,
	CLR_HEADER = 0x0E,
	RESOURCE_TABLE = 0x02,    // 资源表的地址
	EXCEPTION_TABLE = 0x03,   // 异常表的地址
	CERTIFICATE_TABLE = 0x04, // 证书表的地址
	RELOCATION_TABLE = 0x05,  // 基址重定位表
	DEBUGGING_ADDRESS = 0x06, // 调试信息的起始地址
	ARCH_ADDRESS = 0x07,
	GLOBAL_PTR_REGISTER_RVA = 0x08,  // 全局指针寄存器相对虚拟
	TLS_TABLE = 0x09,
	LOAD_CONFIG_TABLE = 0x0A,
};

enum class MODULE_PARTY
{
	USER,
	SYSTEM
};

struct CV_HEADER
{
	DWORD Signature;
	DWORD Offset;
};

struct CV_INFO_PDB20
{
	CV_HEADER CvHeader; //CvHeader.Signature = "NB10"
	DWORD Signature;
	DWORD Age;
	BYTE PdbFileName[1];
};

struct CV_INFO_PDB70
{
	DWORD CvSignature; //"RSDS"
	GUID Signature;
	DWORD Age;
	BYTE PdbFileName[1];
};

struct PDB_VALIDATION
{
	GUID Guid;
	DWORD Signature = 0;
	DWORD Age = 0;

	PDB_VALIDATION()
	{
		memset(&Guid, 0, sizeof(GUID));
	}
};

//
// QRange
//
class QRange : public QPair<quint64, quint64>
{
public:
	QRange(quint64 start, quint64 size) : QPair(start, size) {}

	bool operator==(const QRange& b)
	{
		return this->second < b.first;
	}
};

