#ifndef FRAMECLIENTS_H
#define FRAMECLIENTS_H

#include "MainFrame.h"
#include <QTreeWidget>

namespace Ui
{
class FrameClients;
}

class FrameClients : public MainFrame
{
	Q_OBJECT

public:
	explicit FrameClients(QWidget *parent = 0);
	~FrameClients();

public slots:
	void onAddClicked();
	void refreshData();

private slots:
	void on_actionListEdit_triggered();
	void on_actionListDelete_triggered();
	void on_list_itemActivated(QTreeWidgetItem *item, int);

private:
	SqlCriteria createCriteria();
	void showClientDialog(Client &c);
	Ui::FrameClients *ui;
};

#endif // FRAMECLIENTS_H
