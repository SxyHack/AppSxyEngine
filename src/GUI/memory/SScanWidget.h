#pragma once

#include <QWidget>
#include <QButtonGroup>
#include <QTimer>
#include <QMenu>

#include "ui_SScanWidget.h"
#include "memory_define.h"
#include "SMemorySearch.h"

class SMemoryAddress;

//
// class SScanWidget
// 
class SScanWidget : public QWidget
{
	Q_OBJECT

public:
	SScanWidget(QWidget *parent = Q_NULLPTR);
	~SScanWidget();

	void SetupSignalSlot();

	// Show
	void ShowModules();

	// States
	void ShowStateOpened();
	void ShowStateSearchDone();

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
	void SetupFoundTableMenu();

	void AppendFoundAddress(quint32 row, SMemoryBuffer& buffer);

	EFIND_TYPE GetFindType();
	EFIND_METHOD GetFindMethod();

private slots:
	void OnSelectAllModule(bool checked);
	void OnBaseChanged(QAbstractButton*);
	void OnTimingSearch();
	void OnTimingUpdateFound();

	void OnButtonClickSearch();
	void OnButtonClickRestart();
	void OnButtonClickUndo();
	void OnFindMethodChanged(int);
	void OnRightClickTableFound(const QPoint& pos);
	void OnDClickTableFound(QTableWidgetItem* pItem);

	void OnMenuActionCopyAddress(bool checked);
	void OnMenuActionAppendAddress(bool checked);

public slots:
	void OnSearchDone(SMemoryAction* pAction, quint32 nCount);

signals:
	void sgAppendAddress(SMemoryAddress* pAddress);

protected:
	void showEvent(QShowEvent* e) override;
	void hideEvent(QHideEvent* e) override;

private:
	Ui::SScanWidget ui;
	QString      _InputMask;
	QButtonGroup _ButtonGroup;
	QTimer       _SearchTimer;
	QTimer       _FoundUpdateTimer;

	// 右键菜单
	QMenu        _MenuFound;
	QAction* _MenuActionAppend; // 添加到地址列表
	QAction* _MenuActionRemove; // 删除条目
	QAction* _MenuActionCopyAddress; // 复制地址到剪切板
	QAction* _MenuActionWhatAccess; // 找出是什么访问了这个地址
	QAction* _MenuActionWhatWrite; // 找出是什么改写了这个地址
};
