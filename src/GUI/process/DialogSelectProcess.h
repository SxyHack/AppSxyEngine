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
	void StartEnum(const QString& filter = QString());
	void AppendProcessItem(qint32 row, SProcess* pProcess);

protected:
	void showEvent(QShowEvent* e) override;
	void hideEvent(QHideEvent* e) override;

private slots:
	void OnEnumProcess(qint32 row, SProcess* pProcess);
	void OnEnumProcessDone();

	void OnNameFilterChanged(const QString&);
	void OnItemDoubleClicked(QTableWidgetItem*);
	void OnClickOpenProcess();
	void OnClickCancel();

private:
	Ui::DialogSelectProcess ui;
	DWORD     _SelectProcessID;
	QString   _SelectProcessName;
};
