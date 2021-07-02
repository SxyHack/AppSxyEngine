#pragma once

#include <QDialog>
#include "SxyEngine.h"

#include "ui_DialogSelectProcess.h"

class DialogSelectProcess : public QDialog
{
	Q_OBJECT

public:
	DialogSelectProcess(QWidget *parent = Q_NULLPTR);
	~DialogSelectProcess();

protected:
	void SetupProcessTableHead();


protected:
	void showEvent(QShowEvent* e) override;
	void hideEvent(QHideEvent* e) override;

private slots:
	void OnEnumProcessDone();

private:
	Ui::DialogSelectProcess ui;
	DWORD     _SelectProcessID;
	QString   _SelectProcessName;
};
