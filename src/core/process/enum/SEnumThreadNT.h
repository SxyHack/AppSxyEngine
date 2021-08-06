#pragma once

#include <QThread>

class SProcess;

class SEnumThreadNT : public QThread
{
	Q_OBJECT

public:
	SEnumThreadNT(SProcess* process);
	~SEnumThreadNT();

	void Stop();

protected:
	void run() override;

private:
	SProcess* _Process;
};
