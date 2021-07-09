#pragma once

#include <QThread>
#include <QSemaphore>
#include <QWeakPointer>

class SProcess;

class SEnumModule : public QThread
{
	Q_OBJECT

public:
	SEnumModule(SProcess* process);
	~SEnumModule();

	void Stop();
	//
	// µÈ´ý³õÊ¼»¯
	// 
	void WaitForInit();

protected:
	void run() override;

private:
	SProcess* _Process;
	QSemaphore _ExitSemaphore;
	QSemaphore _InitSemaphore;
};
