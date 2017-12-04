#include "MainFrame.h"
#include "CampingApplication.h"
#include "DialogReservation.h"
#include "main.h"
#include <QDebug>
#include <QMessageBox>

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

SqlCriteria MainFrame::baseCriteria(Location::Type findType)
{
	SqlCriteria criteria;
	QString query;

	if (!this->mainParent()->searchQuery().isEmpty()) {
		query = "%" + this->mainParent()->searchQuery() + "%";
	}

	criteria.setSelect("reservation.*");

	if (findType != Location::ALL || !query.isEmpty()) {
		criteria.setSelect(criteria.select() + ",location.type AS _location_type, location.name AS _location_name, client.name AS _client_name, client.surname AS _client_surname");
		criteria.setJoin("JOIN location ON reservation.location_id = location.id, client ON reservation.client_id = client.id");
		if (findType != Location::ALL) {
			criteria.addCondition("_location_type = :loctype");
			criteria.bindValue(":loctype", findType);
		}
	}

	if (!query.isEmpty()) {
		criteria.setSelect(criteria.select() + ",vehicle.patent AS _vehicle_patent, vehicle.model AS _vehicle_model");
		criteria.setJoin(criteria.join() + " LEFT OUTER JOIN vehicle ON vehicle.reservationt_id = reservation.id");
		criteria.setGroup("reservation.id");
		criteria.addCondition("_client_name LIKE :query1 OR _client_surname LIKE :query2 OR (_client_surname||' '||_client_surname) LIKE :query3 OR _vehicle_patent LIKE :query4 OR _vehicle_model LIKE :query5 OR _location_name LIKE :query6");
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

void MainFrame::doCreateReservation(Reservation &r, Location::Type type)
{
	DialogReservation dialog(&r, type);
	dialog.exec();
	if (dialog.result() == DialogReservation::Accepted) {
		this->requestRefresh();
	}
}

void MainFrame::doEditReservation(Reservation &r)
{
	DialogReservation dialog(&r, r.getLocation().getType());
	dialog.exec();
	if (dialog.result() == DialogReservation::Accepted) {
		this->requestRefresh();
	}
}

void MainFrame::doPrintReceipt(Reservation &r)
{
	App()->printHtml(r.getReceiptHtml(), this);
}

void MainFrame::doDeleteReservation(Reservation &r)
{
	if (QMessageBox::question(
			this,
			tr("Borrando cliente"),
			tr("¿Estás seguro que quieres borrar la reservación de %1?").arg(r.getClient().getFullName()),
			QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
		r.deleteRecord();
		this->requestRefresh();
	}
}
