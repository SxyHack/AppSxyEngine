#pragma once

#include <QString>
#include <QQueue>

#include <windows.h>

// 开启控制台窗口
#define ENABLE_CONSOLE 1
#define ENABLE_DEBUGVIEW 0

class SLogAction
{
public:
	SLogAction(const QString& level, 
		const QString& time, 
		const QString& fileName, 
		const QString& function,
		qint32 fileLine, 
		qint32 threadID);

	~SLogAction();

	void Execute();

private:
	void SetConsoleColor(WORD nColor);
	void ResetConsoleColor();

private:
	QString _Level;
	QString _Time;
	QString _FileName;
	QString _Function;
	qint32  _FileLine;
	qint32  _ThreadID;
	HANDLE  _hSTD;
	WORD    _OriginColors;
};

typedef QQueue<SLogAction*> SQUEUE_LOG;
