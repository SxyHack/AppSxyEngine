#include "SDialogEditAddress.h"

SDialogEditAddress::SDialogEditAddress(SMemoryAddress* pAddress, QWidget* parent)
	: QDialog(parent)
	, _Address(pAddress)
{
	ui.setupUi(this);
	SetupTypes();
	connect(ui.cbBase16, SIGNAL(stateChanged(int)), this, SLOT(OnCheckStateChangedForBase16(int)));
	connect(ui.cbSign, SIGNAL(stateChanged(int)), this, SLOT(OnCheckStateChangedForSign(int)));
	connect(ui.GroupPointer, SIGNAL(toggled(bool)), this, SLOT(OnEnablePointer(bool)));
	connect(ui.ButtonAccept, SIGNAL(clicked()), this, SLOT(OnButtonClickAccept()));
	connect(ui.ButtonCancel, SIGNAL(clicked()), this, SLOT(OnButtonClickCancel()));
	connect(ui.ButtonAppendOffset, SIGNAL(clicked()), this, SLOT(OnButtonClickAppendOffset()));

	ui.txtAddress->setText(pAddress->GetAddressHex());
	ui.txtValue->setText("=" + pAddress->ToString());
}

SDialogEditAddress::~SDialogEditAddress()
{
}

void SDialogEditAddress::SetupTypes()
{
	ui.cbType->addItem("1字节", (qint32)EFIND_TYPE::Byte);
	ui.cbType->addItem("2字节", (qint32)EFIND_TYPE::Byte_2);
	ui.cbType->addItem("4字节", (qint32)EFIND_TYPE::Byte_4);
	ui.cbType->addItem("8字节", (qint32)EFIND_TYPE::Byte_8);
	ui.cbType->addItem("单浮点", (qint32)EFIND_TYPE::Float);
	ui.cbType->addItem("双浮点", (qint32)EFIND_TYPE::Double);
	ui.cbType->addItem("字符串", (qint32)EFIND_TYPE::String);

	ui.cbType->setCurrentIndex((int)_Address->GetType());
}

void SDialogEditAddress::closeEvent(QCloseEvent* e)
{
	_Address->RemoveOffsets();
}

void SDialogEditAddress::OnButtonClickAppendOffset()
{
	auto pLast = _Address->GetLast();
	auto pOffsetWidget = new SAddressOffsetWidget(pLast, ui.GroupPointer);
	connect(pOffsetWidget, SIGNAL(sgDelete()), this, SLOT(OnButtonClickDeleteOffset()));
	connect(pOffsetWidget, SIGNAL(sgInsertBelow()), this, SLOT(OnButtonClickInsertOffset()));
	_OffsetList.append(pOffsetWidget);
	auto nCount = ui.verticalLayout->count();
	ui.verticalLayout->insertWidget(nCount - 1, pOffsetWidget);
}

void SDialogEditAddress::OnButtonClickDeleteOffset()
{
	auto pWidget = qobject_cast<SAddressOffsetWidget*>(QObject::sender());
	if (pWidget)
	{
		ui.verticalLayout->removeWidget(pWidget);
		delete pWidget;
	}
}

void SDialogEditAddress::OnButtonClickInsertOffset()
{
	auto pWidget = qobject_cast<SAddressOffsetWidget*>(QObject::sender());
	if (pWidget)
	{
		auto index = ui.verticalLayout->indexOf(pWidget);
		auto pOffsetWidget = new SAddressOffsetWidget(pWidget->GetMemoryAddress(), ui.GroupPointer);
		connect(pOffsetWidget, SIGNAL(sgDelete()), this, SLOT(OnButtonClickDeleteOffset()));
		connect(pOffsetWidget, SIGNAL(sgInsertBelow()), this, SLOT(OnButtonClickInsertOffset()));
		_OffsetList.append(pOffsetWidget);

		ui.verticalLayout->insertWidget(index + 1, pOffsetWidget);
	}
}

void SDialogEditAddress::OnButtonClickAccept()
{
	_Address->SetDescription(ui.txtDesc->text());
	_Address->SetPointer(ui.GroupPointer->isChecked());
	_Address->SetNewAddress(ui.txtAddress->text().toULongLong(nullptr, 16));
	_Address->SetType(EFIND_TYPE(ui.cbType->currentData().toInt()));

	accept();
}

void SDialogEditAddress::OnButtonClickCancel()
{
	_Address->RemoveOffsets();
	reject();
}

void SDialogEditAddress::OnCheckStateChangedForBase16(int nState)
{
	auto qText = (nState == Qt::Checked)
		? _Address->ToHex()
		: _Address->ToString();
	ui.txtValue->setText("=" + qText);
}

void SDialogEditAddress::OnCheckStateChangedForSign(int nState)
{

}

void SDialogEditAddress::OnEnablePointer(bool bEnabled)
{
	ui.txtAddress->setEnabled(!bEnabled);

	if (bEnabled)
	{
		ui.txtPAddress->setText(ui.txtAddress->text());
		ui.txtPtrTo->setText("->" + _Address->ToHex());
		ui.txtAddress->clear();
	}
	else
	{
		ui.txtAddress->setText(_Address->GetAddressHex());
	}
}
