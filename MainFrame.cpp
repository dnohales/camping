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

SqlCriteria MainFrame::baseCriteria(Location::Type findType)
{
	SqlCriteria criteria;
	QString query;
	
	if(!this->mainParent()->searchQuery().isEmpty()){
		query = "%"+this->mainParent()->searchQuery()+"%";
	}
	
	criteria.setSelect("client.*");
	
	if(findType != Location::ALL || !query.isEmpty()){
		criteria.setSelect(criteria.select()+",location.type AS _location_type, location.name AS _location_name");
		criteria.setJoin("JOIN location ON client.location_id = location.id");
		if(findType != Location::ALL){
			criteria.addCondition("_location_type = :loctype");
			criteria.bindValue(":loctype", findType);
		}
	}
	
	if(!query.isEmpty()){
		criteria.setSelect(criteria.select()+",vehicle.patent AS _vehicle_patent, vehicle.model AS _vehicle_model");
		criteria.setJoin(criteria.join()+" LEFT OUTER JOIN vehicle ON vehicle.client_id = client.id");
		criteria.setGroup("client.id");
		criteria.addCondition("client.name LIKE :query1 OR client.surname LIKE :query2 OR (client.name||' '||client.surname) LIKE :query3 OR _vehicle_patent LIKE :query4 OR _vehicle_model LIKE :query5 OR _location_name LIKE :query6");
		criteria.bindValue(":query1", query);
		criteria.bindValue(":query2", query);
		criteria.bindValue(":query3", query);
		criteria.bindValue(":query4", query);
		criteria.bindValue(":query5", query);
		criteria.bindValue(":query6", query);
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

