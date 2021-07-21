#pragma once

#include <QWidget>
#include "ui_SAddressOffsetWidget.h"
#include "SMemoryAddress.h"

class SAddressOffsetWidget : public QWidget
{
	Q_OBJECT

public:
	SAddressOffsetWidget(SMemoryAddress* pAddress, QWidget *parent = Q_NULLPTR);
	~SAddressOffsetWidget();

	void UpdatePTO();

	SMemoryAddress* GetMemoryAddress();

signals:
	void sgDelete();
	void sgInsertBelow(); // ��������������µ�

private slots:
	void OnOffsetChanged(int nOffset);

private:
	Ui::SAddressOffsetWidget ui;
	SMemoryAddress* _BaseAddress;
	SMemoryAddress* _Address;
};

typedef QList<SAddressOffsetWidget*> SOffsetWidgetList;
