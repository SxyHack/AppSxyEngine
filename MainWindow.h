#pragma once

#include <QtWidgets/QMainWindow>
#include <QTimer>

#include "ui_MainWindow.h"
#include "GUI\memory\QHexView\qhexview.h"
#include "GUI\memory\SScanWidget.h"

class SMemoryAddress;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

protected:
	void SetupToolBar();
	void SetupScanView();
	void SetupAddressList();
	void SetupTableTypes();

	void AppendAddress(SMemoryAddress* pAddress);
	void ShowAddressTypes(SMemoryAddress* pAddress);

protected:
	void closeEvent(QCloseEvent* event) override;

private slots:
	void OnActionOpenProcess(bool checked);
	void OnActionAppendAddress(SMemoryAddress* pAddress);

	// ��ַ�б�ĵ����¼�
	void OnClickAddressList(QTreeWidgetItem* item, int column);
	// ��ַ�б��˫���¼�
	void OnDClickAddressList(QTreeWidgetItem* item, int column);

private:
    Ui::MainWindowClass ui;
	QHexView* _HexView;
	QByteArray _ByteArray;
};
