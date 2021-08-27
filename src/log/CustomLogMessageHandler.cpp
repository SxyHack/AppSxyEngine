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

	//log = QString("%1 %2 %3 %4(%5)")
	//	.arg(level)
	//	.arg(time)
	//	.arg(tid, 6, 10, QLatin1Char('0'))
	//	.arg(file_name)
	//	.arg(ctx.line);
	// 
	//if (!func.isEmpty()) {
	//	log.append(" ");
	//	log.append(func);
	//}
	// 
	//log.append("> ");
	//log.append(msg);
	//log.append("\n");

	gLogMutex.lock();
	gQueue.enqueue(new SLogAction(level, time, file_name, func, ctx.line, tid));
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
