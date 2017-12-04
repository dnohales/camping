#include "Reservation.h"
#include "Vehicle.h"

QString Reservation::tableName()
{
	return "reservation";
}

void Reservation::init()
{
	if (this->isNew()) {
		this->setDateIn(QDate::currentDate());
		this->setDateOut(QDate::currentDate().addDays(1));
		this->setPeopleNum(0);
		this->setTentNum(0);
	}
}

void Reservation::validate()
{
	if (this->getDateOut() < this->getDateIn()) {
		throw ActiveRecordException(tr("La fecha de salida no puede ser menor que la de entrada"));
	}
}

int Reservation::getHousingDays() const
{
	QDateTime dateIn(this->getDateIn());
	QDateTime dateOut(this->getDateOut());
	return dateIn.daysTo(dateOut);
}

bool Reservation::isHousing() const
{
	return this->getDateOut() > QDate::currentDate();
}

VehicleCollection Reservation::getVehicles() const
{
	return Vehicle().findAll(SqlCriteria().addCondition("reservation_id = " + QString::number(this->getId())));
}

ReservationCollection Reservation::getConflictingReservations() const
{
	SqlCriteria criteria;
	criteria.addCondition("id != :id AND location_id = :location_id AND (in_time BETWEEN :di1 AND :do OR (in_time <= :di2 AND out_time > :di3))");
	criteria.bindValue(":id", this->getId());
	criteria.bindValue(":location_id", this->getLocationId());
	criteria.bindValue(":di1", this->getDateIn());
	criteria.bindValue(":do", this->getDateOut().addDays(-1));
	criteria.bindValue(":di2", this->getDateIn());
	criteria.bindValue(":di3", this->getDateIn());

	return Reservation().findAll(criteria);
}

QList<int> ReservationCollection::findByLocationAndDate(const Location &loc, const QDate &date) const
{
	QList<int> indexList;

	for (int i = 0; i < this->count(); i++) {
		Reservation r(this->at(i));
		if (r.getLocationId() == loc.getId() && r.getDateIn() <= date && r.getDateOut() > date) {
			indexList.append(i);
		}
	}

	return indexList;
}

QString Reservation::getReceiptHtml() const
{
	QString html(App()->config()->receiptTemplate());

	html.replace("{nombre}", this->getClient().getName(), Qt::CaseInsensitive)
		.replace("{apellido}", this->getClient().getSurame(), Qt::CaseInsensitive)
		.replace("{dni}", this->getClient().getDni(), Qt::CaseInsensitive)
		.replace("{direccion}", this->getClient().getAdress(), Qt::CaseInsensitive)
		.replace("{cantidad_personas}", QString::number(this->getPeopleNum()), Qt::CaseInsensitive)
		.replace("{vehiculos}", this->getVehicles().toHtml(), Qt::CaseInsensitive)
		.replace("{ubicacion}", this->getLocation().getReadableName(), Qt::CaseInsensitive)
		.replace("{fecha_entrada}", this->getDateIn().toString("dd/MM/yyyy"), Qt::CaseInsensitive)
		.replace("{fecha_salida}", this->getDateOut().toString("dd/MM/yyyy"), Qt::CaseInsensitive);

	if (this->getLocation().getType() != Location::DORM) {
		html.replace("{cantidad_carpas}", QString::number(this->getTentNum()), Qt::CaseInsensitive);
		html.replace("{cantidad_carpas_style}", "visibility:visible;", Qt::CaseInsensitive);
	} else {
		html.replace("{cantidad_carpas_style}", "visibility:hidden;", Qt::CaseInsensitive);
	}

	return html;
}

QString ReservationCollection::toHtmlDocument(QString title) const
{
	QFile htmlFile(":/html/client-collection.html");
	htmlFile.open(QFile::ReadOnly);
	QString html(htmlFile.readAll());
	htmlFile.close();
	QString listString;
	QTextStream list(&listString);

	list << "<table class=\"datatable\">"
		 << "<tr>"
		 << "<th>Cliente</th>"
		 << "<th>F. Entrada</th>"
		 << "<th>F. Salida</th>"
		 << "<th>Ubicaci&oacute;n</th>"
		 << "<th>Personas/Carpas</th>"
		 << "<th>Veh&iacute;culos</th>"
		 << "<th>Se&nacute;a</th>"
		 << "<th>Tel&eacute;fono</th>"
		 << "</tr>";
	for (int i = 0; i < this->count(); i++) {
		Reservation r(this->at(i));
		list << "<tr>";

		list << "<td>" << r.getClient().getFullName() << "</td>";
		list << "<td>" << r.getDateIn().toString("dd/MM/yyyy") << "</td>";
		list << "<td>" << r.getDateOut().toString("dd/MM/yyyy") << "</td>";
		list << "<td>" << r.getLocation().getReadableName() << "</td>";
		list << "<td>" << r.getPeopleNum() << "/" << r.getTentNum() << "</td>";
		list << "<td>" << r.getVehicles().toHtml() << "</td>";
		list << "<td>" << r.getBeck() << "</td>";
		list << "<td>" << r.getClient().getTel() << "</td>";

		list << "</tr>";
	}
	list << "</table>";

	html.replace("{titulo}", title)
		.replace("{clientes}", listString)
		.replace("{pie_de_pagina}", QDateTime::currentDateTime().toString("'Impreso el' d 'de' MMMM 'de' yyyy 'a las' hh:mm"));

	return html;
}

ReservationCollection ReservationCollection::filterByDates(const QDate &dateini, const QDate &dateend) const
{
	ReservationCollection col;

	for (int i = 0; i < this->count(); i++) {
		Reservation r(this->at(i));

		if ((r.getDateIn() >= dateini && r.getDateIn() <= dateend) ||
			(r.getDateOut() >= dateini && r.getDateOut() <= dateend)) {
			col.append(r);
		}
	}

	return col;
}
