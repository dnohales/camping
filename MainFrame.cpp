#include "MainFrame.h"
#include "main.h"
#include "CampingApplication.h"
#include <QDebug>
#include <QMessageBox>
#include "DialogClient.h"

MainFrame::MainFrame(QWidget *parent) :
    QFrame(parent)
{
	this->setRefreshed(false);
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
	this->setRefreshed(true);
}

void MainFrame::requestRefresh()
{
	if(this->isRefreshed() == true){
		this->setRefreshed(false);
		emit refreshed();
	}
}

SqlCriteria MainFrame::baseCriteria()
{
	SqlCriteria criteria;
	
	if(!this->mainParent()->searchQuery().isEmpty()){
		QString query = QString("%")+this->mainParent()->searchQuery()+QString("%");
		criteria.addCondition("client.name LIKE :query OR client.surname LIKE :query OR (client.name||' '||client.surname) LIKE :query");
		criteria.bindValue(":query", query);
	}
	
	return criteria;
}

bool MainFrame::isRefreshed()
{
	return this->_refreshed;
}

void MainFrame::setRefreshed(bool r)
{
	this->_refreshed = r;
}

void MainFrame::doCreateClient(Client &c, Location::Type type)
{
	DialogClient dialog(&c, type);
	dialog.exec();
	if(dialog.result() == DialogClient::Accepted){
		this->requestRefresh();
	}
}

void MainFrame::doEditClient(Client &c)
{
	DialogClient dialog(&c, c.getLocation().getType());
	dialog.exec();
	if(dialog.result() == DialogClient::Accepted){
		this->requestRefresh();
	}
}

void MainFrame::doPrintReceipt(Client &c)
{
	App()->printHtml(c.getReceiptHtml(), this);
}

void MainFrame::doDeleteClient(Client &c)
{
	if(QMessageBox::question(
	            this,
	            tr("Borrando cliente"),
	            tr("¿Estás seguro que quieres borrar a %1?").arg(c.getFullName()),
	            QMessageBox::Yes, QMessageBox::No)
	    == QMessageBox::Yes ){
		c.deleteRecord();
		this->requestRefresh();
	}
}

