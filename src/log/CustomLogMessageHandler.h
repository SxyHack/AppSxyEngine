#pragma once

#include <QObject>

// 开启控制台窗口
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
