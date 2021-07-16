#include "MainWindow.h"
#include "GUI\process\DialogSelectProcess.h"
//#include "GUI\memory\QHexView\document\buffer\qmemorybuffer.h"
#include "GUI\memory\SScanWidget.h"
#include "log\CustomLogMessageHandler.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	SetupToolBar();
	SetupScanView();
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

void MainWindow::SetupScanView()
{
	auto pDefaultTab = new SScanWidget(this);
	auto nDefaultIndex = ui.tabWidget->addTab(pDefaultTab, "默认");
	ui.tabWidget->setCurrentIndex(nDefaultIndex);
	ui.tabWidget->setTabBarAutoHide(true);
}

void MainWindow::SetupAddressList()
{
	
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
		QMessageBox::warning(this, "关联失败", "打开进程失败");
		return;
	}

	auto pProcess = SEngine.GetSelectedProcess();
	if (pProcess == nullptr)
	{
		QMessageBox::warning(this, "关联失败", "选择的进程返回<nullptr>");
		return;
	}

	ui.txtProcessName->setVisible(true);
	ui.txtProcessName->setText(QString("%1(%2)")
		.arg(pProcess->GetFileName())
		.arg(pProcess->GetID()));

	auto pTab = (SScanWidget*)ui.tabWidget->currentWidget();
	if (pTab)
	{
		pTab->SetupSignalSlot();
		pTab->ShowModules();
		pTab->ShowStateOpened();
	}
}