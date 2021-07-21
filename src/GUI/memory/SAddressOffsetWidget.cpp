#include "SAddressOffsetWidget.h"

SAddressOffsetWidget::SAddressOffsetWidget(SMemoryAddress* pAddress, QWidget *parent)
	: QWidget(parent)
	, _BaseAddress(pAddress)
	, _Address(new SMemoryAddress(pAddress))
{
	_BaseAddress->SetNext(_Address);

	ui.setupUi(this);
	UpdatePTO();

	connect(ui.ButtonDelete,      SIGNAL(clicked()), this, SIGNAL(sgDelete()));
	connect(ui.ButtonInsertBelow, SIGNAL(clicked()), this, SIGNAL(sgInsertBelow()));
	connect(ui.Offset, SIGNAL(valueChanged(int)), this, SLOT(OnOffsetChanged(int)));
}

SAddressOffsetWidget::~SAddressOffsetWidget()
{
}

void SAddressOffsetWidget::UpdatePTO()
{
	auto nOffset = ui.Offset->value();
	if (_BaseAddress->Read(nOffset))
	{
		auto sBasePTO = _BaseAddress->ToHex();
		auto sPTO = QString("->%1=(%2+%3)")
			.arg(sBasePTO)
			.arg(_BaseAddress->GetAddressHex())
			.arg(QString::number(nOffset, 16).toUpper());
		ui.OffsetPTO->setText(sPTO);
		ui.ButtonInsertBelow->setEnabled(true);
	}
	else
	{
		ui.OffsetPTO->setText("->????????");
		ui.ButtonInsertBelow->setEnabled(false);
	}
}

SMemoryAddress* SAddressOffsetWidget::GetMemoryAddress()
{
	return _Address;
}

void SAddressOffsetWidget::OnOffsetChanged(int nOffset)
{
	UpdatePTO();
}
