#ifndef CLIENT_H
#define CLIENT_H

#include "ActiveRecord.h"
#include "Location.h"
#include "Vehicle.h"
#include <QList>
#include <QAbstractListModel>

class Client;

class ClientCollection : public QList<Client>
{
public:
	/**
	 * Devuelve una lista de IDs de los clientes en esta colección que están ocupando
	 * la ubicación "loc" y serán huéspedes en la fecha "date".
	 */
	QList<int> findByLocationAndDate(const Location &loc, const QDate &date) const;
	QString toHtmlDocument(QString title = "Lista de clientes") const;
	ClientCollection filterByDates(const QDate &dateini, const QDate &dateend) const;
};

class Client : public ActiveRecord
{
	ACTIVE_RECORD(Client, ClientCollection)

public:
	QString tableName();
	void validate();

	QString getFullName() const;
	Location getLocation() const;
	void setLocation(const Location &loc);
	int getHousingDays() const;
	bool isHousing() const;
	VehicleCollection getVehicles() const;
	ClientCollection getConflictingClients() const;
	QString getReceiptHtml() const;

	ACTIVE_RECORD_FIELD_STRING(getName, setName, "name")
	ACTIVE_RECORD_FIELD_STRING(getSurame, setSurame, "surname")
	ACTIVE_RECORD_FIELD(getDateIn, setDateIn, QDate, "in_time")
	ACTIVE_RECORD_FIELD(getDateOut, setDateOut, QDate, "out_time")
	ACTIVE_RECORD_FIELD(getLocationId, setLocationId, int, "location_id")
	ACTIVE_RECORD_FIELD_STRING(getEmail, setEmail, "email")
	ACTIVE_RECORD_FIELD_STRING(getTel, setTel, "tel")
	ACTIVE_RECORD_FIELD_STRING(getBeck, setBeck, "cel")
	ACTIVE_RECORD_FIELD_STRING(getDni, setDni, "dni")
	ACTIVE_RECORD_FIELD_STRING(getAdress, setAddress, "address")
	ACTIVE_RECORD_FIELD(getPeopleNum, setPeopleNum, int, "people_num")
	ACTIVE_RECORD_FIELD(getTentNum, setTentNum, int, "tent_num")

protected:
	void init();
};

#endif // CLIENT_H
