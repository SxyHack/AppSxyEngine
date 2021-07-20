#pragma once

#include <QDialog>
#include "ui_SDialogEditAddress.h"
#include "SAddressOffsetWidget.h"

class SDialogEditAddress : public QDialog
{
	Q_OBJECT

public:
	SDialogEditAddress(SMemoryAddress* pAddress, QWidget *parent = Q_NULLPTR);
	~SDialogEditAddress();

protected:
	void SetupTypes();
	void UpdateOffsets();

private slots:
	void OnButtonClickAppendOffset();
	void OnButtonClickDeleteOffset();
	void OnButtonClickInsertOffset();

	void OnButtonClickAccept();
	void OnButtonClickCancel();

	void OnCheckStateChangedForBase16(int nState);
	void OnCheckStateChangedForSign(int nState);
	void OnEnablePointer(bool bEnabled);

private:
	Ui::SDialogEditAddress ui;
	SMemoryAddress* _Address;
	SOffsetWidgetList _OffsetList;
};
