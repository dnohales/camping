#ifndef FRAMEDORMS_H
#define FRAMEDORMS_H

#include "FrameReservations.h"
#include <QFrame>
#include <QTableWidget>

namespace Ui
{
class FrameDorms;
}

class FrameDorms : public FrameReservations
{
	Q_OBJECT

public:
	explicit FrameDorms(QWidget *parent = 0);
	~FrameDorms();

public slots:
	void refreshData();

protected:
	void resizeEvent(QResizeEvent *);

private:
	void refreshComboYears(ReservationCollection &clist);

private slots:
	void onMenuCreate();
	void onMenuEdit();
	void onMenuPrint();
	void onMenuDelete();
	void on_buttonAdd_clicked();
	void on_table_itemActivated(QTableWidgetItem *item);
	void on_table_customContextMenuRequested(QPoint pos);
	void onTableColumnWidthUpdate();

private:
	QTableWidgetItem *selectedItem;
	Ui::FrameDorms *ui;
};

#endif // FRAMEDORMS_H
