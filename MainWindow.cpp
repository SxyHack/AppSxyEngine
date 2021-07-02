#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	SetupToolBar();
}

void MainWindow::SetupToolBar()
{
	QAction* actOpenProcess = new QAction(QIcon(":/M/resources/Icons/openprocess128x128.png"), tr("&Open..."), this);
	actOpenProcess->setStatusTip("打开进程");
	actOpenProcess->setToolTip("打开进程");
	ui.mainToolBar->addAction(actOpenProcess);
	//connect(actOpenProcess, &QAction::triggered, this, &MainWindow::OnActionOpenProcess);

	//QAction* actMemory = new QAction(QIcon(":/MAIN/resources/Icons/edit128x128.png"), tr("&Memory"), this);
	//actMemory->setStatusTip("显示内存视图");
	//actMemory->setToolTip("显示内存视图");
	//ui.mainToolBar->addAction(actMemory);
	//connect(actMemory, &QAction::triggered, this, &MainWindow::OnActionOpenMemory);
}
