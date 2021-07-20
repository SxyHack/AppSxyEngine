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
	// ת����16����
	// 
	void ChangeToBase16();
	//
	// ת����10����
	//
	void ChangeToBase10();
	//
	// ת����2����
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

	// �Ҽ��˵�
	QMenu        _MenuFound;
	QAction* _MenuActionAppend; // ��ӵ���ַ�б�
	QAction* _MenuActionRemove; // ɾ����Ŀ
	QAction* _MenuActionCopyAddress; // ���Ƶ�ַ�����а�
	QAction* _MenuActionWhatAccess; // �ҳ���ʲô�����������ַ
	QAction* _MenuActionWhatWrite; // �ҳ���ʲô��д�������ַ
};
