#ifndef LOCATION_H
#define LOCATION_H

#include "ActiveRecord.h"
#include <QAbstractListModel>

class Location;

class LocationCollection : public QList<Location>
{
};

class LocationModel : public QAbstractListModel
{
     Q_OBJECT

public:
	LocationModel(const LocationCollection &col, QObject *parent = 0)
	 : QAbstractListModel(parent), collection(col) {}
	
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role) const;
	
private:
	LocationCollection collection;
};

class Location : public ActiveRecord
{
	ACTIVE_RECORD(Location, LocationCollection)
	
	enum Type {
		TENT = 0,
		DORMI = 1
	};
	
public:
	QString tableName();
	Location findByNameType(QString name, Location::Type type);
	LocationCollection findAllByType(Location::Type type);
	void validate();
	
	ACTIVE_RECORD_FIELD_STRING(getName, setName, "name")
	Location::Type getType();
	void setType(Location::Type value);
	
protected:
	void init();
};

#endif // LOCATION_H
