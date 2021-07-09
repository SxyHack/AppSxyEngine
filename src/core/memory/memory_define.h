#pragma once
//
// �ڴ��ֵ������, ���ַ�����N�ֽ������ȵ� 
//
enum class EFIND_TYPE
{
	Byte,
	Byte_2,
	Byte_4,
	Byte_8,
	Float,
	Double,
	String,
	All
};


enum class EFIND_METHOD
{
	Exact,        // ��ȷ����
	MoreThan,     // ��������ֵ��
	LessThan,     // ��������ֵС
	Between,      // ����[a, b]֮��
	Unknown,      // δ֪����ֵ

	Bigger,
	Smaller,
	IncreaseN,
	DecreaseN,
	Changed,
	Unchanged,
	EqualBase
};

enum class EVALUE_BASE
{
	Base2,
	Base10,
	Base16
};
