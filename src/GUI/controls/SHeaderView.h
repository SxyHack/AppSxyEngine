#pragma once

#include <QHeaderView>

//
// 自定义QTableWidget表头, 在第一列上显示全选控件
//
class SHeaderView : public QHeaderView
{
	Q_OBJECT

public:
	SHeaderView(QWidget* parent = nullptr);
	~SHeaderView();

protected:
	void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const override;
	void mouseReleaseEvent(QMouseEvent* e) override;

signals:
	void sgStateChanged(bool checked);

protected:
	bool _EnableSelectAll;
	bool _IsSelected;
};
