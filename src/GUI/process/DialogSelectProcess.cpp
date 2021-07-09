#include "DialogSelectProcess.h"

DialogSelectProcess::DialogSelectProcess(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	SetupProcessTableHead();

	connect(ui.NameFilter, SIGNAL(textChanged(const QString&)), this, SLOT(OnNameFilterChanged(const QString&)));
	connect(ui.OpenBtn, SIGNAL(clicked()), this, SLOT(OnClickOpenProcess()));
	connect(ui.CancelBtn, SIGNAL(clicked()), this, SLOT(OnClickCancel()));
	connect(ui.ProcessList, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(OnItemDoubleClicked(QTableWidgetItem*)));
}

DialogSelectProcess::~DialogSelectProcess()
{
}

void DialogSelectProcess::SetupProcessTableHead()
{
	// 设置表头参数
	QStringList tableHead;
	tableHead << "PID" << "进程名称";

	ui.ProcessList->setColumnCount(tableHead.length());
	ui.ProcessList->setColumnWidth(0, 120);
	ui.ProcessList->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	ui.ProcessList->setColumnWidth(2, 50);
	ui.ProcessList->setAlternatingRowColors(true);

	for (qint32 i = 0; i < tableHead.length(); i++)
	{
		QTableWidgetItem* headItem = new QTableWidgetItem(tableHead[i]);
		QFont font = headItem->font();
		font.setBold(true);

		headItem->setTextColor(Qt::gray);
		headItem->setFont(font);
		ui.ProcessList->setHorizontalHeaderItem(i, headItem);
	}
}

void DialogSelectProcess::StartEnum(const QString& filter)
{
	ui.ProcessList->clearContents();
	ui.ProcessList->setRowCount(1024);
	SEngine.EnumProcess(filter);
}

void DialogSelectProcess::AppendProcessItem(qint32 row, SProcess* pProcess)
{
	QString qsProcessID = QString("%1").arg(pProcess->GetID(), 8, 10, QLatin1Char('0'));

	auto item0 = new QTableWidgetItem(qsProcessID);
	item0->setTextAlignment(Qt::AlignCenter);
	item0->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	item0->setData(Qt::UserRole, QVariant::fromValue(pProcess));

	auto item1 = new QTableWidgetItem(pProcess->GetFileName());
	item1->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	item1->setData(Qt::UserRole, QVariant::fromValue(pProcess));
	//item1->setIcon(pProcess->GetIcon());

	ui.ProcessList->setItem(row, 0, item0);
	ui.ProcessList->setItem(row, 1, item1);
}

void DialogSelectProcess::showEvent(QShowEvent* e)
{
	connect(&SEngine, SIGNAL(sgEnumProcess(qint32, SProcess*)), this, SLOT(OnEnumProcess(qint32, SProcess*)), Qt::QueuedConnection);
	connect(&SEngine, SIGNAL(sgEnumProcessDone()), this, SLOT(OnEnumProcessDone()));
	StartEnum();
}

void DialogSelectProcess::hideEvent(QHideEvent* e)
{
	disconnect(&SEngine, SIGNAL(sgEnumProcess(qint32, SProcess*)), this, SLOT(OnEnumProcess(qint32, SProcess*)));
	disconnect(&SEngine, SIGNAL(sgEnumProcessDone()), this, SLOT(OnEnumProcessDone()));
}

void DialogSelectProcess::OnEnumProcess(qint32 row, SProcess* pProcess)
{
	AppendProcessItem(row, pProcess);
}

void DialogSelectProcess::OnEnumProcessDone()
{
	auto processes = SEngine.GetProcessList();
	ui.ProcessList->setRowCount(processes.count());
	ui.ProcessList->scrollToBottom();
}

void DialogSelectProcess::OnNameFilterChanged(const QString& text)
{
	StartEnum(text);
}

void DialogSelectProcess::OnItemDoubleClicked(QTableWidgetItem* item)
{
	auto pProcess = item->data(Qt::UserRole).value<SProcess*>();
	if (pProcess == nullptr)
		return;

	SEngine.SelectProcess(pProcess);
	accept();
}

void DialogSelectProcess::OnClickOpenProcess()
{
	auto items = ui.ProcessList->selectedItems();
	if (items.isEmpty())
		return;

	auto item = items.first();
	auto pProcess = item->data(Qt::UserRole).value<SProcess*>();
	if (pProcess == nullptr)
		return;

	SEngine.SelectProcess(pProcess);
	accept();
}

void DialogSelectProcess::OnClickCancel()
{
	reject();
}
