#ifndef MAINFRAME_H
#define MAINFRAME_H

#include "MainWindow.h"
#include "Reservation.h"
#include "SqlCriteria.h"
#include <QFrame>

class MainFrame : public QFrame
{
	Q_OBJECT
public:
	explicit MainFrame(QWidget *parent = 0);

	void setMainParent(MainWindow *m);
	MainWindow *mainParent();

	bool isRefreshed();

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
