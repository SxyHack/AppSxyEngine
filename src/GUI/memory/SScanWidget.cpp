#include "SScanWidget.h"
#include "SMemoryBuffer.h"
#include "GUI/controls/SHeaderView.h"
#include "SxyEngine.h"
#include "SFindMethodExact.h"

#include <QMessageBox>

SScanWidget::SScanWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	_ButtonGroup.addButton(ui.Base2, (qint32)EVALUE_BASE::Base2);
	_ButtonGroup.addButton(ui.Base10, (qint32)EVALUE_BASE::Base10);
	_ButtonGroup.addButton(ui.Base16, (qint32)EVALUE_BASE::Base16);
	_InputMask = ui.TargetValueA->inputMask();

	SetupFoundTable();
	SetupModuleTable();

	SetupFindWhat();
	SetupFindMethod();

	// 默认
	ui.TargetValueA->setValidator(new QIntValidator(_I32_MIN, _I32_MAX));
	ui.TargetValueB->setValidator(new QIntValidator(_I32_MIN, _I32_MAX));
	ui.TargetValueB->setVisible(false);

	ui.ButtonSearch->setEnabled(false);
	ui.ButtonRestart->setEnabled(false);
	ui.ButtonUndo->setEnabled(false);
	ui.FindType->setEnabled(false);
	ui.FindMethod->setEnabled(false);

	// connections
	connect(&_ButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
		this, SLOT(OnBaseChanged(QAbstractButton*)));
	connect(ui.FindMethod, SIGNAL(currentIndexChanged(int)), this, SLOT(OnFindMethodChanged(int)));
	connect(ui.ButtonSearch, SIGNAL(clicked()), this, SLOT(OnButtonClickSearch()));
	connect(ui.ButtonRestart, SIGNAL(clicked()), this, SLOT(OnButtonClickRestart()));
	connect(ui.ButtonUndo, SIGNAL(clicked()), this, SLOT(OnButtonClickUndo()));

	// Timer
	connect(&_SearchTimer, SIGNAL(timeout()), this, SLOT(OnTimingSearch()));
	connect(&_FoundUpdateTimer, SIGNAL(timeout()), this, SLOT(OnTimingUpdateFound()));
}

SScanWidget::~SScanWidget()
{
}

void SScanWidget::ChangeToBase16()
{

}

void SScanWidget::ChangeToBase10()
{

}

void SScanWidget::ChangeToBase2()
{

}

void SScanWidget::SetupFindWhat()
{
	ui.FindType->addItem("1字节", (qint32)EFIND_TYPE::Byte);
	ui.FindType->addItem("2字节", (qint32)EFIND_TYPE::Byte_2);
	ui.FindType->addItem("4字节", (qint32)EFIND_TYPE::Byte_4);
	ui.FindType->addItem("8字节", (qint32)EFIND_TYPE::Byte_8);
	ui.FindType->addItem("单浮点", (qint32)EFIND_TYPE::Float);
	ui.FindType->addItem("双浮点", (qint32)EFIND_TYPE::Double);
	ui.FindType->addItem("字符串", (qint32)EFIND_TYPE::String);
	ui.FindType->addItem("全部", (qint32)EFIND_TYPE::All);

	ui.FindType->setCurrentIndex(2);
}

void SScanWidget::SetupFindMethod()
{
	ui.FindMethod->clear();
	ui.FindMethod->addItem("精确数值", (qint32)EFIND_METHOD::Exact);
	ui.FindMethod->addItem("大于输入值", (qint32)EFIND_METHOD::MoreThan);
	ui.FindMethod->addItem("小于输入值", (qint32)EFIND_METHOD::LessThan);
	ui.FindMethod->addItem("介于..两数之间", (qint32)EFIND_METHOD::Between);
	ui.FindMethod->addItem("未知初始值", (qint32)EFIND_METHOD::Unknown);

	ui.FindMethod->setCurrentIndex(0); // 默认使用精确查找
}

void SScanWidget::ShowStateSearchDone()
{
	ui.FindMethod->removeItem(ui.FindMethod->count() - 1);
	ui.FindMethod->addItem("数值增加了N", (qint32)EFIND_METHOD::IncreaseN);
	ui.FindMethod->addItem("数值减少了N", (qint32)EFIND_METHOD::DecreaseN);
	ui.FindMethod->addItem("增加的数值", (qint32)EFIND_METHOD::Bigger);
	ui.FindMethod->addItem("减少的数值", (qint32)EFIND_METHOD::Smaller);
	ui.FindMethod->addItem("变动的数值", (qint32)EFIND_METHOD::Changed);
	ui.FindMethod->addItem("未变的数值", (qint32)EFIND_METHOD::Unchanged);
	ui.FindMethod->addItem("对比首次搜索", (qint32)EFIND_METHOD::EqualBase);
}


void SScanWidget::SetupFoundTable()
{
	QStringList tableHead;
	tableHead << "地址" << "当前值" << "先前值";
	ui.TableFound->setColumnCount(tableHead.length());
	ui.TableFound->setColumnWidth(0, 200);
	ui.TableFound->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
	ui.TableFound->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	ui.TableFound->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
	ui.TableFound->setAlternatingRowColors(true);

	for (qint32 i = 0; i < tableHead.length(); i++)
	{
		auto headItem = new QTableWidgetItem(tableHead[i]);
		headItem->setTextColor(Qt::lightGray);
		headItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		ui.TableFound->setHorizontalHeaderItem(i, headItem);
	}
}

void SScanWidget::AppendFoundAddress(quint32 row, SMemoryBuffer& buffer)
{
	QString qModuleName;
	auto nOffset = SEngine.QueryModuleOffset(qModuleName, buffer.Address);

	auto qAddress = nOffset >= 0
		? QString("%1+%2").arg(qModuleName).arg(QString::number(nOffset, 16)).toUpper()
		: QString::number(buffer.Address, 16).toUpper();
	auto itemAddr = new QTableWidgetItem(qAddress);
	itemAddr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	itemAddr->setTextColor(nOffset >= 0 ? Qt::darkGreen : Qt::white);
	itemAddr->setData(Qt::UserRole, QVariant::fromValue(buffer));
	if (nOffset >= 0)
	{
		itemAddr->setToolTip(QString::number(buffer.Address, 16).toUpper());
	}
	ui.TableFound->setItem(row, 0, itemAddr);

	auto qsValue = buffer.ToString();
	auto itemValue = new QTableWidgetItem(qsValue);
	itemValue->setData(Qt::UserRole, QVariant::fromValue(buffer));
	ui.TableFound->setItem(row, 1, itemValue);

	auto itemCopy = new QTableWidgetItem(qsValue);
	itemCopy->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	ui.TableFound->setItem(row, 2, itemCopy);
}


void SScanWidget::SetupModuleTable()
{
	// 设置表头参数
	QStringList tableHead;
	tableHead << "模块" << "起始" << "结束";

	SHeaderView* header = new SHeaderView(this);
	ui.TableModule->setHorizontalHeader(header);
	ui.TableModule->setColumnCount(tableHead.length());
	ui.TableModule->setColumnWidth(0, 150);
	ui.TableModule->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	ui.TableModule->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
	ui.TableModule->setAlternatingRowColors(true);

	connect(header, SIGNAL(sgStateChanged(bool)), this, SLOT(OnSelectAllModule(bool)));

	for (qint32 i = 0; i < tableHead.length(); i++)
	{
		auto headItem = new QTableWidgetItem(tableHead[i]);
		QFont font = headItem->font();
		font.setBold(true);

		headItem->setFont(font);
		headItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsAutoTristate);
		if (i == 0)
		{
			headItem->setCheckState(Qt::PartiallyChecked);
		}

		ui.TableModule->setHorizontalHeaderItem(i, headItem);
	}
}


EFIND_TYPE SScanWidget::GetFindType()
{
	return EFIND_TYPE(ui.FindType->currentData().toInt());
}

EFIND_METHOD SScanWidget::GetFindMethod()
{
	return EFIND_METHOD(ui.FindMethod->currentData().toInt());
}

void SScanWidget::ShowModules()
{
	auto pProcess = SEngine.GetSelectedProcess();
	if (pProcess == nullptr)
	{
		return;
	}

	auto nModuleCount = pProcess->GetModuleCount();
	qint32 nReverseRow = nModuleCount - 1;
	qint32 nRow = 0;

	ui.TableModule->setRowCount(nModuleCount);

	for (int i = 0; i < nModuleCount; i++)
	{
		auto pModule = pProcess->GetModule(i);
		if (pModule == nullptr)
			continue;
		bool bSystemDLL = (pModule->Party == MODULE_PARTY::SYSTEM);
		if (pModule->FileExt.contains("exe", Qt::CaseInsensitive))
			bSystemDLL = false;

		QColor color = bSystemDLL ? Qt::gray : Qt::green;
		qint32 nPos = bSystemDLL ? nReverseRow-- : nRow++;

		auto item0 = new QTableWidgetItem(pModule->FileName);
		item0->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
		item0->setTextColor(color);
		item0->setToolTip(pModule->FilePath);
		item0->setCheckState(bSystemDLL ? Qt::Unchecked : Qt::Checked);
		item0->setData(Qt::UserRole, QVariant::fromValue(pModule));

		ui.TableModule->setItem(nPos, 0, item0);

		auto qsBaseAddr = QString::number((quint64)pModule->ModBase, 16).toUpper();
		auto item1 = new QTableWidgetItem(qsBaseAddr);
		item1->setTextColor(color);
		item1->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);

		ui.TableModule->setItem(nPos, 1, item1);

		auto dwEndAddr = (quint64)pModule->ModBase + pModule->ModSize;
		auto qsEndAddr = QString::number(dwEndAddr, 16).toUpper();
		auto itEndAddr = new QTableWidgetItem(qsEndAddr);
		itEndAddr->setTextColor(color);
		itEndAddr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
		ui.TableModule->setItem(nPos, 2, itEndAddr);
	}
}

void SScanWidget::ShowStateOpened()
{
	ui.ButtonSearch->setEnabled(true);
	ui.ButtonRestart->setEnabled(false);
	ui.ButtonUndo->setEnabled(false);

	ui.FindType->setEnabled(true);
	ui.FindMethod->setEnabled(true);
	ui.TableFound->clearContents();
	ui.TableFound->setRowCount(0);
}


void SScanWidget::SetupSignalSlot()
{
	auto pProcess = SEngine.GetSelectedProcess();
	if (pProcess)
	{
		connect(pProcess, &SProcess::sgSearchDone, this, &SScanWidget::OnSearchDone, Qt::QueuedConnection);
	}
}

void SScanWidget::OnSelectAllModule(bool checked)
{
	for (int i = 0; i < ui.TableModule->rowCount(); i++)
	{
		auto item = ui.TableModule->item(i, 0);
		auto pModule = item->data(Qt::UserRole).value<SModule*>();

		if (checked)
		{
			item->setCheckState(Qt::Checked);
		}
		else
		{
			if (pModule && pModule->Party == MODULE_PARTY::SYSTEM)
			{
				item->setCheckState(Qt::Unchecked);
			}
		}
	}
}

void SScanWidget::OnBaseChanged(QAbstractButton* pButton)
{

}

void SScanWidget::OnTimingSearch()
{
	auto pProcess = SEngine.GetSelectedProcess();
	if (pProcess == nullptr)
	{
		return;
	}

	ui.ScanProgress->setMaximum(pProcess->NumberOfVirtualMemory);
	ui.ScanProgress->setValue(pProcess->NumberOfSearch);
	//quint64 nReadedBytes = 0;
	//quint64 nTotalBytes = 0;
	//pProcess->GetSearchProgress(nReadedBytes, nTotalBytes);
	//quint64 nPercentage = double(nReadedBytes) / double(nTotalBytes) * 100;
	//ui.ScanProgress->setValue(nPercentage);
}

void SScanWidget::OnTimingUpdateFound()
{
	auto maxSize = ui.TableFound->maximumViewportSize();
	auto row = ui.TableFound->visualRow(0);

	for (int r = 0; r < ui.TableFound->rowCount(); r++)
	{
		auto pos = ui.TableFound->rowViewportPosition(r);
		if (0 <= pos && pos <= ui.TableFound->height())
		{
			auto pItem1 = ui.TableFound->item(r, 1);
			auto pItem2 = ui.TableFound->item(r, 2);
			auto& buff = pItem1->data(Qt::UserRole).value<SMemoryBuffer>();
			buff.Update();
			auto newValue = buff.ToString();
			pItem1->setText(newValue);
			pItem1->setTextColor(newValue == pItem2->text() ? Qt::white : Qt::yellow);
		}
	}
}

void SScanWidget::OnButtonClickSearch()
{
	auto pProcess = SEngine.GetSelectedProcess();
	if (pProcess == nullptr)
	{
		QMessageBox::warning(this, "不能搜索", "没有选择进程");
		return;
	}

	auto type = EFIND_TYPE(ui.FindType->currentData().toInt());
	auto method = EFIND_METHOD(ui.FindMethod->currentData().toInt());
	auto va = ui.TargetValueA->text();
	auto vb = ui.TargetValueB->text();
	pProcess->Search(type, method, va, vb);

	// 启动进度条更新计时器
	ui.ScanProgress->setValue(0);
	ui.ScanProgress->setVisible(true);
	ui.TargetValueA->setEnabled(false);
	ui.TargetValueB->setEnabled(false);
	ui.FindType->setEnabled(false);
	ui.FindMethod->setEnabled(false);
	ui.ButtonSearch->setEnabled(false);

	_SearchTimer.start(10);
	_FoundUpdateTimer.stop();
}

void SScanWidget::OnButtonClickRestart()
{
	ShowStateOpened();
	SetupFindMethod();
	SEngine.Restart();
}

void SScanWidget::OnButtonClickUndo()
{

}

void SScanWidget::OnFindMethodChanged(int index)
{
	switch (GetFindMethod())
	{
	case EFIND_METHOD::Between:
		ui.TargetValueA->setEnabled(true);
		ui.TargetValueB->setVisible(true);
		break;
	case EFIND_METHOD::Changed:
	case EFIND_METHOD::Unchanged:
	case EFIND_METHOD::EqualBase:
	case EFIND_METHOD::Bigger:
	case EFIND_METHOD::Smaller:
		ui.TargetValueA->setEnabled(false);
		ui.TargetValueB->setVisible(false);
		break;
	default:
		ui.TargetValueA->setEnabled(true);
		ui.TargetValueB->setVisible(false);
		break;
	};
}

void SScanWidget::OnSearchDone(SMemoryAction* pAction, quint32 nCount)
{
	ui.TargetValueA->setEnabled(true);
	ui.TargetValueB->setEnabled(true);
	ui.FindMethod->setEnabled(true);
	ui.ButtonSearch->setEnabled(true);
	ui.ButtonRestart->setEnabled(true);
	ui.ScanProgress->setValue(0);
	_SearchTimer.stop();

	ShowStateSearchDone();

	if (pAction == nullptr)
		return;

	const static auto MAX_SHOW_COUNT = 5000;
	auto nRow = 0;

	if (nCount > MAX_SHOW_COUNT)
	{
		ui.FoundCount->setText(QString("显示%1(%2)").arg(MAX_SHOW_COUNT).arg(nCount));
		nCount = MAX_SHOW_COUNT;
	}
	else
	{
		ui.FoundCount->setText(QString::number(nCount));
	}

	ui.TableFound->setRowCount(nCount);
	for (auto& what : pAction->GetWhatList())
	{
		for (int i = 0; i < what.GetFoundCount(); i++)
		{
			auto buff = what.GetBuffer(i);
			AppendFoundAddress(nRow++, buff);
			if (nRow >= nCount)
				goto SEARCH_DONE_END;
		}
	}

SEARCH_DONE_END:
	_FoundUpdateTimer.start(200);
}

void SScanWidget::showEvent(QShowEvent* e)
{

}

void SScanWidget::hideEvent(QHideEvent* e)
{

}
