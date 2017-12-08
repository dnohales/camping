#ifndef FRAMETENTS_H
#define FRAMETENTS_H

#include "FrameReservations.h"
#include <QFrame>
#include <QTreeWidget>

namespace Ui
{
class FrameTents;
}

class FrameTents : public FrameReservations
{
	Q_OBJECT

public:
	explicit FrameTents(QWidget *parent);
	~FrameTents();
	ReservationCollection currentList();

public slots:
	void onAddClicked();
	void refreshData();

private slots:
	void on_actionListEdit_triggered();
	void on_actionListPrint_triggered();
	void on_actionListDelete_triggered();
	void on_list_itemActivated(QTreeWidgetItem *item, int column);

private:
	ReservationCollection _currentList;
	Ui::FrameTents *ui;
};

#endif // FRAMETENTS_H
