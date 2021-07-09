#pragma once

#include <QtWidgets/QMainWindow>
#include <QTimer>

#include "ui_MainWindow.h"
#include "GUI\memory\QHexView\qhexview.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

protected:
	void SetupToolBar();
	void SetupHexView();
	void SetupScanView();

protected:
	void closeEvent(QCloseEvent* event) override;


private slots:
	void OnActionOpenProcess(bool checked);
	void OnTimeTick();

private:
    Ui::MainWindowClass ui;
	QHexView* _HexView;
	QByteArray _ByteArray;
};
