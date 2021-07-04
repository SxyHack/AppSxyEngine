#include "MainWindow.h"
#include "GUI\process\DialogSelectProcess.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	SetupToolBar();
	SetupHexView();
}

void MainWindow::SetupToolBar()
{
	QAction* actOpenProcess = new QAction(QIcon(":/M/resources/Icons/openprocess128x128.png"), tr("&Open..."), this);
	actOpenProcess->setStatusTip("打开进程");
	actOpenProcess->setToolTip("打开进程");
	ui.mainToolBar->addAction(actOpenProcess);
	connect(actOpenProcess, &QAction::triggered, this, &MainWindow::OnActionOpenProcess);

	//QAction* actMemory = new QAction(QIcon(":/MAIN/resources/Icons/edit128x128.png"), tr("&Memory"), this);
	//actMemory->setStatusTip("显示内存视图");
	//actMemory->setToolTip("显示内存视图");
	//ui.mainToolBar->addAction(actMemory);
	//connect(actMemory, &QAction::triggered, this, &MainWindow::OnActionOpenMemory);
}

void MainWindow::SetupHexView()
{
	_HexView = new QHexView(this);
	_HexView->setReadOnly(true);

	setCentralWidget(_HexView);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	QMainWindow::closeEvent(event);
}

void MainWindow::OnActionOpenProcess(bool checked)
{
	DialogSelectProcess dialog;

	if (dialog.exec() == QDialog::Rejected)
	{
		return;
	}

	if (!SEngine.AttachSelectedProcess())
	{
		qWarning("关联进程失败");
		QMessageBox::warning(this, "", "关联进程失败");
		return;
	}


}
