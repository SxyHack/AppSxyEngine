#include "DialogSelectProcess.h"

DialogSelectProcess::DialogSelectProcess(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

DialogSelectProcess::~DialogSelectProcess()
{
}

void DialogSelectProcess::SetupProcessTableHead()
{
	// 设置表头参数
	QStringList tableHead;
	tableHead << "PID" << "进程名称";

	//ui.ProcessList->setColumnCount(tableHead.length());
	//ui.ProcessList->setColumnWidth(0, 120);
	//ui.ProcessList->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	//ui.ProcessList->setColumnWidth(2, 50);
	//ui.ProcessList->setAlternatingRowColors(true);

	//for (qint32 i = 0; i < tableHead.length(); i++)
	//{
	//	QTableWidgetItem* headItem = new QTableWidgetItem(tableHead[i]);
	//	QFont font = headItem->font();
	//	font.setBold(true);

	//	headItem->setTextColor(Qt::gray);
	//	headItem->setFont(font);
	//	ui.ProcessList->setHorizontalHeaderItem(i, headItem);
	//}

}

void DialogSelectProcess::showEvent(QShowEvent* e)
{

}

void DialogSelectProcess::hideEvent(QHideEvent* e)
{

}
