#ifndef CLIENT_H
#define CLIENT_H

#include "ActiveRecord.h"

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

	QString getFullName() const;

	ACTIVE_RECORD_FIELD_STRING(getName, setName, "name")
	ACTIVE_RECORD_FIELD_STRING(getSurame, setSurame, "surname")
	ACTIVE_RECORD_FIELD_STRING(getEmail, setEmail, "email")
	ACTIVE_RECORD_FIELD_STRING(getTel, setTel, "tel")
	ACTIVE_RECORD_FIELD_STRING(getDni, setDni, "dni")
	ACTIVE_RECORD_FIELD_STRING(getAdress, setAddress, "address")
};

#endif // CLIENT_H
