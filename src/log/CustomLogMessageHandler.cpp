#include "CustomLogMessageHandler.h"

#include <QMutex>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QThread>
#include <QDir>
#include <QTextStream>
#include <iostream>
#include <windows.h>

static QMutex gMutex;
static QMutex gLogMutex;
static QFile  gLogFile;
static FILE*  gFile;


CustomLogMessageHandler::CustomLogMessageHandler()
{
#if ENABLE_CONSOLE
	AllocConsole();
	freopen_s(&gFile, "CONOUT$", "w", stdout);
#endif
}

CustomLogMessageHandler& CustomLogMessageHandler::Instance()
{
	QMutexLocker lock(&gMutex);
	static CustomLogMessageHandler instance;
	return instance;
}

void CustomLogMessageHandler::handle(QtMsgType type, const QMessageLogContext& ctx, const QString& msg)
{
	QMutexLocker lock(&gLogMutex);

	if (msg.isEmpty())
		return;

	QString log;
	QString func("");
	QFileInfo file_info(ctx.file);
	auto file_name = file_info.fileName();
	auto now = QDateTime::currentDateTime();
	auto time = now.toString("hh:mm:ss.zzz");
	auto tid = (qint64)QThread::currentThreadId();
	auto level = "U";

	switch (type)
	{
	case QtDebugMsg:
		level = "D";
		break;
	case QtWarningMsg:
		level = "W";
		break;
	case QtCriticalMsg:
		level = "E";
		break;
	case QtFatalMsg:
		level = "F";
		func = ctx.function;
		break;
	case QtInfoMsg:
		level = "I";
		break;
	}

	log = QString("%1 %2 %3 %4(%5)")
		.arg(level)
		.arg(tid)
		.arg(time)
		.arg(file_name)
		.arg(ctx.line);

	if (!func.isEmpty()) {
		log.append(" ");
		log.append(func);
	}

	log.append("> ");
	log.append(msg);
	log.append("\n");

	if (!gLogFile.isOpen()) {
		auto dir = QDir::current();
		dir.mkdir("logs");
		dir.cd("logs");

		QString log_file_name = QString("%1.log")
			.arg(now.toString("yyyy-MM-dd-HH"));
		auto log_file_dir = dir.absoluteFilePath(log_file_name);
		gLogFile.setFileName(log_file_dir);
		gLogFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
	}

	QTextStream stm(&gLogFile);
	stm.setCodec("UTF-8");
	stm << log;
	stm.flush();

#if ENABLE_CONSOLE
	std::cout << log.toLocal8Bit().constData();
#elif ENABLE_DEBUGVIEW
#ifdef _UNICODE
	TCHAR tszLog[1024] = { 0 };
	log.toWCharArray(tszLog);
	OutputDebugString(tszLog);
#else
	OutputDebugString(log.toLocal8Bit().constData());
#endif
#endif
}
