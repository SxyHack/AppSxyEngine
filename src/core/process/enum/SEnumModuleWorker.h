#pragma once

#include <QThread>
#include <QSemaphore>
#include <QWeakPointer>

class SProcess;

class SEnumModuleWorker : public QThread
{
	Q_OBJECT

public:
	SEnumModuleWorker(SProcess* process);
	~SEnumModuleWorker();

	void Stop();
	//
	// �ȴ���ʼ��
	// 
	void WaitForInit();

protected:
	void run() override;

private:
	SProcess* _Process;
	QSemaphore _ExitSemaphore;
	QSemaphore _InitSemaphore;
};
