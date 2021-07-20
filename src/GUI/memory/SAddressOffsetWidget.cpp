#include "SAddressOffsetWidget.h"

SAddressOffsetWidget::SAddressOffsetWidget(SMemoryAddress* pAddress, QWidget *parent)
	: QWidget(parent)
	, _Address(pAddress)
{
	ui.setupUi(this);
	connect(ui.ButtonDelete, SIGNAL(clicked()), this, SIGNAL(sgDelete()));
	connect(ui.ButtonInsertBelow, SIGNAL(clicked()), this, SIGNAL(sgInsertBelow()));

}

SAddressOffsetWidget::~SAddressOffsetWidget()
{
}

void SAddressOffsetWidget::UpdateOffsetPtr(int nIndex)
{
}
