#pragma once

#include <QThread>

class SProcess;

//
//
//
class SEnumProcess : public QThread
{
	Q_OBJECT

public:
	SEnumProcess(const QString& filter);
	~SEnumProcess();

protected:
	void run() override;

signals:
	void sgEnumProcessDone();

private:
	QString _Filter;
};
