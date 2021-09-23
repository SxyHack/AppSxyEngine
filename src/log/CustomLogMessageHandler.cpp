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
static FILE*  gFile;

static SQUEUE_LOG gQueue;

CustomLogMessageHandler::CustomLogMessageHandler()
{
#if ENABLE_CONSOLE
	AllocConsole();
	freopen_s(&gFile, "CONOUT$", "w", stdout);
	HWND hWnd = GetConsoleWindow();
	MoveWindow(hWnd, 0, 0, 800, 1000, TRUE);
#endif
}

CustomLogMessageHandler& CustomLogMessageHandler::Instance()
{
	QMutexLocker lock(&gMutex);
	static CustomLogMessageHandler instance;
	instance.start();
	return instance;
}

void CustomLogMessageHandler::handle(QtMsgType type, const QMessageLogContext& ctx, const QString& msg)
{
	if (msg.isEmpty())
		return;

	QString log;
	QString func;
	QFileInfo file_info(ctx.file);
	auto file_name = file_info.fileName();
	auto now = QDateTime::currentDateTime();
	auto time = now.toString("hh:mm:ss.zzz");
	auto tid = (qint64)QThread::currentThreadId();
	auto level = "U";
	auto color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
	switch (type)
	{
	case QtDebugMsg:
		level = "D";
		break;
	case QtWarningMsg:
		level = "W";
		color = FOREGROUND_RED | FOREGROUND_GREEN;
		break;
	case QtCriticalMsg:
		level = "E";
		color = FOREGROUND_RED;
		break;
	case QtFatalMsg:
		level = "F";
		func = ctx.function;
		color = FOREGROUND_RED | FOREGROUND_INTENSITY;
		break;
	case QtInfoMsg:
		level = "I";
		color = FOREGROUND_GREEN;
		break;
	}

	gLogMutex.lock();
	gQueue.enqueue(new SLogAction(color, level, time, file_name, func, msg, ctx.line, tid));
	gLogMutex.unlock();
}

void CustomLogMessageHandler::Stop()
{
	if (isInterruptionRequested())
		return;

	requestInterruption();
	if (_ExitSemaphore.tryAcquire(1, 5000))
		_ExitSemaphore.release();
}

void CustomLogMessageHandler::run()
{
	while (!isInterruptionRequested())
	{
		if (gQueue.isEmpty())
		{
			QThread::msleep(1);
			continue;
		}

		auto pAction = gQueue.dequeue();
		pAction->Execute();
		delete pAction;
	}

	_ExitSemaphore.release();
}
