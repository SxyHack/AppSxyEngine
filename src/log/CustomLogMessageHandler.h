#pragma once

#include <QObject>

// ��������̨����
#define ENABLE_CONSOLE 1
#define ENABLE_DEBUGVIEW 0


class CustomLogMessageHandler
{
public:
	static CustomLogMessageHandler& Instance();
	static void handle(QtMsgType type, const QMessageLogContext& ctx, const QString& msg);
	
private:
	CustomLogMessageHandler();
};
