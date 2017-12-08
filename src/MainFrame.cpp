#include "MainFrame.h"

MainFrame::MainFrame(QWidget *parent)
	: QFrame(parent)
{
	this->setRefreshed(false);
}

void MainFrame::setMainParent(MainWindow *m)
{
	this->_parent = m;
}

MainWindow *MainFrame::mainParent()
{
	return this->_parent;
}

void MainFrame::refreshData()
{
	this->setRefreshed(true);
}

void MainFrame::requestRefresh()
{
	if (this->isRefreshed() == true) {
		this->setRefreshed(false);
		emit refreshed();
	}
}

bool MainFrame::isRefreshed()
{
	return this->_refreshed;
}

void MainFrame::setRefreshed(bool r)
{
	this->_refreshed = r;
}
