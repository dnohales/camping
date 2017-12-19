#ifndef RESERVATION_H
#define RESERVATION_H

#include "ActiveRecord.h"
#include "Client.h"
#include "Location.h"

class VehicleCollection;
class Reservation;

class ReservationCollection : public QList<Reservation>
{
public:
	/**
	 * Devuelve una lista de IDs de los clientes en esta colección que están ocupando
	 * la ubicación "loc" y serán huéspedes en la fecha "date".
	 */
	QList<int> findByLocationAndDate(const Location &loc, const QDate &date) const;
	QString toHtmlDocument(QString title) const;
	ReservationCollection filterByDates(const QDate &dateini, const QDate &dateend) const;
};

class Reservation : public ActiveRecord
{
	ACTIVE_RECORD(Reservation, ReservationCollection)

public:
	QString tableName();
	void validate();

	int getHousingDays() const;
	bool isHousing() const;
	VehicleCollection getVehicles() const;
	ClientCollection getConflictingClients() const;
	QString getReceiptHtml() const;
	ReservationCollection getConflictingReservations() const;

	ACTIVE_RECORD_MANY_TO_ONE(getClient, setClient, Client, "client_id")
	ACTIVE_RECORD_MANY_TO_ONE(getLocation, setLocation, Location, "location_id")
	ACTIVE_RECORD_FIELD(getDateIn, setDateIn, QDate, "in_time")
	ACTIVE_RECORD_FIELD(getDateOut, setDateOut, QDate, "out_time")
	ACTIVE_RECORD_FIELD(getPeopleNum, setPeopleNum, int, "people_num")
	ACTIVE_RECORD_FIELD(getTentNum, setTentNum, int, "tent_num")
	ACTIVE_RECORD_FIELD_STRING(getBeck, setBeck, "beck")

protected:
	void init();
};

#endif // RESERVATION_H
