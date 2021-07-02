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
	actOpenProcess->setStatusTip("�򿪽���");
	actOpenProcess->setToolTip("�򿪽���");
	ui.mainToolBar->addAction(actOpenProcess);
	//connect(actOpenProcess, &QAction::triggered, this, &MainWindow::OnActionOpenProcess);

	//QAction* actMemory = new QAction(QIcon(":/MAIN/resources/Icons/edit128x128.png"), tr("&Memory"), this);
	//actMemory->setStatusTip("��ʾ�ڴ���ͼ");
	//actMemory->setToolTip("��ʾ�ڴ���ͼ");
	//ui.mainToolBar->addAction(actMemory);
	//connect(actMemory, &QAction::triggered, this, &MainWindow::OnActionOpenMemory);
}
