#include "FrameReservations.h"
#include "DialogReservation.h"
#include "main.h"
#include <QDebug>
#include <QMessageBox>

FrameReservations::FrameReservations(QWidget *parent)
	: MainFrame(parent)
{
}

SqlCriteria FrameReservations::baseCriteria(Location::Type findType)
{
	SqlCriteria criteria;
	QString query;

	if (!this->mainParent()->searchQuery().isEmpty()) {
		query = "%" + this->mainParent()->searchQuery() + "%";
	}

	criteria.setSelect("reservation.*");

	if (findType != Location::ALL || !query.isEmpty()) {
		criteria.setSelect(criteria.select() + ", location.type AS _location_type, location.name AS _location_name");
		criteria.setJoin("JOIN location ON reservation.location_id = location.id");
		if (findType != Location::ALL) {
			criteria.addCondition("_location_type = :loctype");
			criteria.bindValue(":loctype", findType);
		}
	}

	if (!query.isEmpty()) {
		criteria.setSelect(criteria.select() + ", client.name AS _client_name, client.surname AS _client_surname, vehicle.patent AS _vehicle_patent, vehicle.model AS _vehicle_model");
		criteria.setJoin(criteria.join() + " JOIN client ON reservation.client_id = client.id LEFT OUTER JOIN vehicle ON vehicle.reservation_id = reservation.id");
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

void FrameReservations::doCreateReservation(Reservation &r, Location::Type type)
{
	DialogReservation dialog(&r, type);
	dialog.exec();
	if (dialog.result() == DialogReservation::Accepted) {
		this->requestRefresh();
	}
}

void FrameReservations::doEditReservation(Reservation &r)
{
	DialogReservation dialog(&r, r.getLocation().getType());
	dialog.exec();
	if (dialog.result() == DialogReservation::Accepted) {
		this->requestRefresh();
	}
}

void FrameReservations::doPrintReceipt(Reservation &r)
{
	App()->printHtml(r.getReceiptHtml(), this);
}

void FrameReservations::doDeleteReservation(Reservation &r)
{
	if (QMessageBox::question(
			this,
			tr("Borrando reservación"),
			tr("¿Estás seguro que quieres borrar la reservación de %1?").arg(r.getClient().getFullName()),
			QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
		r.deleteRecord();
		this->requestRefresh();
	}
}
