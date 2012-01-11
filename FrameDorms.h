#ifndef FRAMEDORMS_H
#define FRAMEDORMS_H

#include <QFrame>
#include "MainFrame.h"
#include <QTableWidget>

namespace Ui {
    class FrameDorms;
}

class FrameDorms : public MainFrame
{
    Q_OBJECT

public:
    explicit FrameDorms(QWidget *parent = 0);
    ~FrameDorms();
	
public slots:
	void refreshData();
	
private:
	void refreshComboMonths(ClientCollection &clist);

private slots:
	void onMenuCreate();
	void onMenuEdit();
	void onMenuPrint();
	void onMenuDelete();
	void on_buttonAdd_clicked();
	void on_table_itemActivated(QTableWidgetItem* item);
	void on_table_customContextMenuRequested(QPoint pos);
	
private:
	QTableWidgetItem *selectedItem;
    Ui::FrameDorms *ui;
};

#endif // FRAMEDORMS_H
