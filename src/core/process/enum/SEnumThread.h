#pragma once

#include <QThread>

class SProcess;

//
// class SEnumThread
//
class SEnumThread : public QThread
{
	Q_OBJECT

public:
	SEnumThread(SProcess* process);
	~SEnumThread();

	void Stop();

protected:
	void run() override;

private:
	SProcess* _Process;
};
