#pragma once

#include <QThread>

class SThread;

class SThreadRiper : public QThread
{
	Q_OBJECT

public:
	SThreadRiper(SThread* pThread);
	~SThreadRiper();

protected:
	void run() override;
};
