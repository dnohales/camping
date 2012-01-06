#ifndef CLIENT_H
#define CLIENT_H

#include "ActiveRecord.h"
#include "Location.h"
#include <QList>

class Client;

class ClientCollection : public QList<Client>
{
};

class Client : public ActiveRecord
{
	ACTIVE_RECORD(Client, ClientCollection)
	
public:
	QString tableName();
	void validate();
	
	QString getFullName();
	Location getLocation();
	void setLocation(const Location &loc);
	
	ACTIVE_RECORD_FIELD_STRING(getName, setName, "name")
	ACTIVE_RECORD_FIELD_STRING(getSurame, setSurame, "surname")
	ACTIVE_RECORD_FIELD(getDateIn, setDateIn, QDate, "in_time")
	ACTIVE_RECORD_FIELD(getDateOut, setDateOut, QDate, "out_time")
	ACTIVE_RECORD_FIELD(getLocationId, setLocationId, uint, "location_id")
	ACTIVE_RECORD_FIELD_STRING(getEmail, setEmail, "email")
	ACTIVE_RECORD_FIELD_STRING(getTel, setTel, "tel")
	ACTIVE_RECORD_FIELD_STRING(getCel, setCel, "cel")
	ACTIVE_RECORD_FIELD_STRING(getDni, setDni, "dni")
	ACTIVE_RECORD_FIELD_STRING(getAdress, setAddress, "address")
	ACTIVE_RECORD_FIELD(getPeopleNum, setPeopleNum, int, "people_num")
	ACTIVE_RECORD_FIELD(getTentNum, setTentNum, int, "tent_num")
	
protected:
	void init();
};

#endif // CLIENT_H
