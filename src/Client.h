#ifndef CLIENT_H
#define CLIENT_H

#include "ActiveRecord.h"
#include <QAbstractListModel>

class Client;
class ReservationCollection;

class ClientCollection : public QList<Client>
{
};

class ClientCompleterModel : public QAbstractListModel
{
public:
	ClientCompleterModel(const ClientCollection &collection);
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	int rowCount(const QModelIndex &) const;

private:
	ClientCollection collection;
};

class Client : public ActiveRecord
{
	ACTIVE_RECORD(Client, ClientCollection)

public:
	QString tableName();
	void validate();

	QString getFullName() const;
	ReservationCollection getReservations() const;

	ACTIVE_RECORD_FIELD_STRING(getName, setName, "name")
	ACTIVE_RECORD_FIELD_STRING(getSurame, setSurame, "surname")
	ACTIVE_RECORD_FIELD_STRING(getEmail, setEmail, "email")
	ACTIVE_RECORD_FIELD_STRING(getTel, setTel, "tel")
	ACTIVE_RECORD_FIELD_STRING(getDni, setDni, "dni")
	ACTIVE_RECORD_FIELD_STRING(getAdress, setAddress, "address")
};

#endif // CLIENT_H
