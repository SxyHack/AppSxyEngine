#pragma once
//
// 内存块值的类型, 如字符串，N字节整数等等 
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
	Exact,        // 精确查找
	MoreThan,     // 比输入数值大
	LessThan,     // 比输入数值小
	Between,      // 介于[a, b]之间
	Unknown,      // 未知的数值

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
