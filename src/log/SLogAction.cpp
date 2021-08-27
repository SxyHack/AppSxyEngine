#include "SLogAction.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDateTime>
#include <iostream>
#include <windows.h>

QFile gLogFile;

SLogAction::SLogAction(const QString& level,
	const QString& time, 
	const QString& fileName, 
	const QString& function,
	qint32 fileLine, 
	qint32 threadID)
	: _Level(level)
	, _Time(time)
	, _FileName(fileName)
	, _FileLine(fileLine)
	, _Function(function)
	, _ThreadID(threadID)
{
	_hSTD = GetStdHandle(STD_OUTPUT_HANDLE);
}

SLogAction::~SLogAction()
{
}

void SLogAction::Execute()
{
	if (!gLogFile.isOpen()) {
		auto now = QDateTime::currentDateTime();
		auto dir = QDir::current();
		dir.mkdir("logs");
		dir.cd("logs");

		QString qLogFileName = QString("%1.log")
			.arg(now.toString("yyyy-MM-dd-HH-mm-ss"));
		auto qLogFilePath = dir.absoluteFilePath(qLogFileName);
		gLogFile.setFileName(qLogFilePath);
		gLogFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
	}

	QTextStream stm(&gLogFile);
	stm.setCodec("UTF-8");
	stm.flush();

#if ENABLE_CONSOLE
	std::cout << _Message.toLocal8Bit().constData();
#elif ENABLE_DEBUGVIEW
#ifdef _UNICODE
	TCHAR tszLog[1024] = { 0 };
	_Message.toWCharArray(tszLog);
	OutputDebugString(tszLog);
#else
	OutputDebugString(_Message.toLocal8Bit().constData());
#endif
#endif
}

void SLogAction::SetConsoleColor(WORD nColor)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	SecureZeroMemory(&csbi, sizeof(CONSOLE_SCREEN_BUFFER_INFO));

	/* Save the original console color */
	GetConsoleScreenBufferInfo(_hSTD, &csbi);
	_OriginColors = *(&csbi.wAttributes);

	SetConsoleTextAttribute(_hSTD, 13);
}

