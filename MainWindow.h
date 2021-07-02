#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

protected:
	void SetupToolBar();

protected:
	void closeEvent(QCloseEvent* event) override;


private slots:
	void OnActionOpenProcess(bool checked);

private:
    Ui::MainWindowClass ui;
};
