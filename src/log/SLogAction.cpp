#include "SLogAction.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDateTime>
#include <iostream>
#include <windows.h>

QFile gLogFile;

SLogAction::SLogAction(const QString& msg)
	: _Message(msg)
{
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
	stm << _Message;
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

