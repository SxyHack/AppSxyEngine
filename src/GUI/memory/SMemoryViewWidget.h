#pragma once

#include <QWidget>

class SMemoryViewWidget : public QWidget
{
	Q_OBJECT

public:
	SMemoryViewWidget(QWidget *parent);
	~SMemoryViewWidget();

protected:
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

private:
	bool _AddressAreaVisible;
	
};
