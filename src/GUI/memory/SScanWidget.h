#pragma once

#include <QWidget>
#include <QButtonGroup>

#include "ui_SScanWidget.h"
#include "memory_define.h"

class SScanWidget : public QWidget
{
	Q_OBJECT

public:
	SScanWidget(QWidget *parent = Q_NULLPTR);
	~SScanWidget();

	// Show
	void ShowModules();

	// States
	void ShowStateOpened();

protected:
	//
	// 转换到16进制
	// 
	void ChangeToBase16();
	//
	// 转换到10进制
	//
	void ChangeToBase10();
	//
	// 转换到2进制
	//
	void ChangeToBase2();

	// Setup
	void SetupFindWhat();
	void SetupFindMethod();
	void SetupFoundTable();
	void SetupModuleTable();

	EFIND_TYPE GetFindType();
	EFIND_METHOD GetFindMethod();

private slots:
	void OnSelectAllModule(bool checked);
	void OnBaseChanged(QAbstractButton*);

	void OnButtonClickSearch();
	void OnButtonClickRestart();
	void OnButtonClickUndo();

protected:
	void showEvent(QShowEvent* e) override;
	void hideEvent(QHideEvent* e) override;

private:
	Ui::SScanWidget ui;
	QButtonGroup _ButtonGroup;
	QString      _InputMask;
};
