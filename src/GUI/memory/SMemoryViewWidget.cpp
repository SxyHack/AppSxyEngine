#include "SMemoryViewWidget.h"
#include <QPaintEvent>
#include <QPainter>

SMemoryViewWidget::SMemoryViewWidget(QWidget *parent)
	: QWidget(parent)
{
}

SMemoryViewWidget::~SMemoryViewWidget()
{
}

void SMemoryViewWidget::paintEvent(QPaintEvent* event)
{
	QRect rect = event->rect();
}

void SMemoryViewWidget::resizeEvent(QResizeEvent* event)
{

}
