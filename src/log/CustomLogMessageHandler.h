#pragma once

#include <QThread>
#include <QQueue>
#include <QSemaphore>

//#include "SLogAction.h"



class CustomLogMessageHandler : public QThread
{
public:
	static CustomLogMessageHandler& Instance();
	static void handle(QtMsgType type, const QMessageLogContext& ctx, const QString& msg);
	
	void Stop();

protected:
	void run() override;

private:
	CustomLogMessageHandler();

	QSemaphore _ExitSemaphore;
};
