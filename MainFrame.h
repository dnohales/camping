#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <QFrame>
#include "SqlCriteria.h"
#include "MainWindow.h"
#include "Client.h"

class MainFrame : public QFrame
{
	Q_OBJECT
public:
	explicit MainFrame(QWidget *parent = 0);

	void setMainParent(MainWindow *m);
	MainWindow *mainParent();

	SqlCriteria baseCriteria(Location::Type findType = Location::ALL);
	bool isRefreshed();

	void doCreateClient(Client &c, Location::Type type);
	void doEditClient(Client &c);
	void doPrintReceipt(Client &c);
	void doDeleteClient(Client &c);

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
