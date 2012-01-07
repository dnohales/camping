#include "MainFrame.h"
#include <QDebug>

MainFrame::MainFrame(QWidget *parent) :
    QFrame(parent)
{
}

void MainFrame::setMainParent(MainWindow *m)
{
	this->_parent = m;
}

MainWindow * MainFrame::mainParent()
{
	return this->_parent;
}

void MainFrame::refreshData()
{
}

SqlCriteria MainFrame::baseCriteria()
{
	SqlCriteria criteria;
	
	if(!this->mainParent()->searchQuery().isEmpty()){
		QString query = QString("%")+this->mainParent()->searchQuery()+QString("%");
		criteria.addCondition("name LIKE :query OR surname LIKE :query OR (name||' '||surname) LIKE :query");
		criteria.bindValue(":query", query);
	}
	
	return criteria;
}
