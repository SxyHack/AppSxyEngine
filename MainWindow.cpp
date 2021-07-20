#include "MainWindow.h"
#include "GUI\process\DialogSelectProcess.h"
#include "GUI\memory\SScanWidget.h"
#include "GUI\memory\SDialogEditAddress.h"
#include "core\memory\SMemoryAddress.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	SetupToolBar();
	SetupScanView();
	SetupAddressList();
	SetupTableTypes();
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

	connect(pDefaultTab, SIGNAL(sgAppendAddress(SMemoryAddress*)), 
		this, SLOT(OnActionAppendAddress(SMemoryAddress*)));
}

void MainWindow::SetupAddressList()
{
	QStringList tableHead;
	tableHead << "激活" << "地址" << "类型" << "数值" << "描述";
	ui.TreeAddress->setColumnCount(tableHead.length());
	ui.TreeAddress->setColumnWidth(0, 64);
	ui.TreeAddress->header()->setSectionResizeMode(0, QHeaderView::Fixed);
	ui.TreeAddress->header()->setSectionResizeMode(1, QHeaderView::Stretch);
	ui.TreeAddress->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
	ui.TreeAddress->header()->setSectionResizeMode(3, QHeaderView::Stretch);
	ui.TreeAddress->header()->setSectionResizeMode(4, QHeaderView::Stretch);
	ui.TreeAddress->setAlternatingRowColors(true);
	ui.TreeAddress->setHeaderLabels(tableHead);

	connect(ui.TreeAddress, &QTreeWidget::itemClicked, this, &MainWindow::OnClickAddressList);
	connect(ui.TreeAddress, &QTreeWidget::itemDoubleClicked, this, &MainWindow::OnDClickAddressList);
}

void MainWindow::SetupTableTypes()
{
	QStringList tableHead;
	tableHead << "类型" << "值";
	ui.TableTypes->setColumnCount(tableHead.length());
	ui.TableTypes->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
	ui.TableTypes->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	ui.TableTypes->setAlternatingRowColors(true);

	for (qint32 i = 0; i < tableHead.length(); i++)
	{
		auto headItem = new QTableWidgetItem(tableHead[i]);
		headItem->setTextColor(Qt::lightGray);
		headItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		ui.TableTypes->setHorizontalHeaderItem(i, headItem);
	}
}

void MainWindow::AppendAddress(SMemoryAddress* pAddress)
{
	auto item0 = new QTreeWidgetItem();
	item0->setFlags(item0->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	item0->setCheckState(0, Qt::Unchecked);
	item0->setData(0, Qt::UserRole, QVariant::fromValue(pAddress));
	ui.TreeAddress->addTopLevelItem(item0);

	item0->setText(1, QString::number(pAddress->GetAddress(), 16).toUpper());
	item0->setText(2, pAddress->GetTypeFormatString());
	item0->setText(3, pAddress->ToString());
	item0->setText(4, "");
}

void MainWindow::ShowAddressTypes(SMemoryAddress* pAddress)
{
	if (pAddress == nullptr)
		return;

	quint32 nBase = 10;
	quint8  nByte = pAddress->ToByte();
	quint16 nByte2 = 0;
	quint32 nByte4 = 0;
	quint64 nByte8 = 0;
	float   nFloat = 0;
	double  nDouble = 0;

	// 十六进制
	ui.TableTypes->setRowCount(7);
	ui.TableTypes->setItem(0, 0, new QTableWidgetItem("HEX"));
	ui.TableTypes->setItem(0, 1, new QTableWidgetItem(pAddress->ToHex()));

	// 单字节
	ui.TableTypes->setItem(1, 0, new QTableWidgetItem("字节"));
	ui.TableTypes->setItem(1, 1, new QTableWidgetItem(QString::number(nByte, nBase)));

	// 2字节
	ui.TableTypes->setItem(2, 0, new QTableWidgetItem("2字节"));
	if (pAddress->ToNumber<quint16>(nByte2))
		ui.TableTypes->setItem(2, 1, new QTableWidgetItem(QString::number(nByte2, nBase)));
	else
		ui.TableTypes->setItem(2, 1, new QTableWidgetItem("??"));

	// 4字节
	ui.TableTypes->setItem(3, 0, new QTableWidgetItem("4字节"));
	if (pAddress->ToNumber<quint32>(nByte4))
		ui.TableTypes->setItem(3, 1, new QTableWidgetItem(QString::number(nByte4, nBase)));
	else
		ui.TableTypes->setItem(3, 1, new QTableWidgetItem("??"));

	// 8字节
	ui.TableTypes->setItem(4, 0, new QTableWidgetItem("8字节"));
	if (pAddress->ToNumber<quint64>(nByte8))
		ui.TableTypes->setItem(4, 1, new QTableWidgetItem(QString::number(nByte8, nBase)));
	else
		ui.TableTypes->setItem(4, 1, new QTableWidgetItem("??"));

	// float
	ui.TableTypes->setItem(5, 0, new QTableWidgetItem("单浮点"));
	if (pAddress->ToNumber<float>(nFloat))
		ui.TableTypes->setItem(5, 1, new QTableWidgetItem(QString::number(nFloat)));
	else
		ui.TableTypes->setItem(5, 1, new QTableWidgetItem("??"));

	// double
	ui.TableTypes->setItem(6, 0, new QTableWidgetItem("双浮点"));
	if (pAddress->ToNumber<double>(nDouble))
		ui.TableTypes->setItem(6, 1, new QTableWidgetItem(QString::number(nDouble)));
	else
		ui.TableTypes->setItem(6, 1, new QTableWidgetItem("??"));
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

void MainWindow::OnActionAppendAddress(SMemoryAddress* pAddress)
{
	AppendAddress(pAddress);
}

void MainWindow::OnClickAddressList(QTreeWidgetItem* item, int column)
{
	auto pAddress = item->data(0, Qt::UserRole).value<SMemoryAddress*>();
	ShowAddressTypes(pAddress);
}

void MainWindow::OnDClickAddressList(QTreeWidgetItem* item, int column)
{
	auto pAddress = item->data(0, Qt::UserRole).value<SMemoryAddress*>();
	SDialogEditAddress dialog(pAddress);
	dialog.exec();
}
