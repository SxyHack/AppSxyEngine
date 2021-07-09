#pragma once

#include <QObject>
#include <QRunnable>

class SAbstractAction : public QObject, public QRunnable
{
	Q_OBJECT

public:
	SAbstractAction(QObject *parent);
	~SAbstractAction();
};
