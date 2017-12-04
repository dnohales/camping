#ifndef MAINFRAME_H
#define MAINFRAME_H

#include "Reservation.h"
#include "MainWindow.h"
#include "SqlCriteria.h"
#include <QFrame>

class MainFrame : public QFrame
{
	Q_OBJECT
public:
	explicit MainFrame(QWidget *parent = 0);

	void setMainParent(MainWindow *m);
	MainWindow *mainParent();

	SqlCriteria baseCriteria(Location::Type findType = Location::ALL);
	bool isRefreshed();

	void doCreateReservation(Reservation &c, Location::Type type);
	void doEditReservation(Reservation &c);
	void doPrintReceipt(Reservation &c);
	void doDeleteReservation(Reservation &c);

protected:
	void setRefreshed(bool r);

public slots:
	virtual void refreshData();
	void requestRefresh();

signals:
	void refreshed();

private:
	bool _refreshed;
	MainWindow *_parent;
};

#endif // MAINFRAME_H
