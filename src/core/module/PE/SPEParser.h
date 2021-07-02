#pragma once

#include <QObject>
#include "global.h"

class SModule;

class SPEParser : public QObject
{
	Q_OBJECT

public:
	SPEParser(SModule* pModule);
	~SPEParser();

	bool Execute();

private:
	NTSTATUS ParseDOSHead();
	NTSTATUS ParseNTHead();
	NTSTATUS ParseSection();

private:
	SModule* _Module;
};
