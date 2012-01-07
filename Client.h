#ifndef CLIENT_H
#define CLIENT_H

#include "ActiveRecord.h"
#include "Location.h"
#include <QList>
#include <QAbstractListModel>

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
	Location getLocation() const;
	void setLocation(const Location &loc);
	int getHousingDays() const;
	bool isHousing() const;
	
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

class ClientModel : public QAbstractListModel
{
     Q_OBJECT

public:
	ClientModel(const ClientCollection &col, QObject *parent = 0)
	 : QAbstractListModel(parent), collection(col) {}
	
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation,
	                         int role = Qt::DisplayRole) const;
	
private:
	ClientCollection collection;
};

#endif // CLIENT_H
