#ifndef CLIENT_H
#define CLIENT_H

#include "ActiveRecord.h"
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
	
	ACTIVE_RECORD_FIELD(getName, setName, QString, "name")
	ACTIVE_RECORD_FIELD(getSurame, setSurame, QString, "surname")
	ACTIVE_RECORD_FIELD(getEmail, setEmail, QString, "email")
	ACTIVE_RECORD_FIELD(getTel, setTel, QString, "tel")
	ACTIVE_RECORD_FIELD(getCel, setCel, QString, "cel")
	ACTIVE_RECORD_FIELD(getDni, setDni, QString, "dni")
	ACTIVE_RECORD_FIELD(getAdress, setAddress, QString, "address")
	ACTIVE_RECORD_FIELD(getPeopleNum, setPeopleNum, int, "people_num")
	ACTIVE_RECORD_FIELD(getTentNum, setTentNum, int, "tent_num")
	
protected:
	void init();
};

#endif // CLIENT_H
