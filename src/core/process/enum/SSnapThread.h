#pragma once

#include <QThread>

class SProcess;

class SSnapThread : public QThread
{
	Q_OBJECT

public:
	SSnapThread(SProcess* process);
	~SSnapThread();
	void Stop();

protected:
	void run() override;

private:
	SProcess* _Process;

};
