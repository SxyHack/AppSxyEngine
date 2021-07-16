#pragma once

#include <QString>
#include <QQueue>

// 开启控制台窗口
#define ENABLE_CONSOLE 1
#define ENABLE_DEBUGVIEW 0

class SLogAction
{
public:
	SLogAction(const QString& msg);
	~SLogAction();

	void Execute();

private:
	QString _Message;
};

typedef QQueue<SLogAction*> SQUEUE_LOG;
