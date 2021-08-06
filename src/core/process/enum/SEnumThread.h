#pragma once

#include <QThread>
#include "global.h"

class SProcess;

//
// class SEnumThread
//
class SEnumThread : public QThread
{
	Q_OBJECT

public:
	SEnumThread(SProcess* process, HANDLE hSnap = NULL);
	~SEnumThread();

	void Stop();

protected:
	void run() override;

private:
	SProcess* _Process;
	HANDLE    _hSnap;
};
