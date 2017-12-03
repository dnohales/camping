#ifndef LOCATION_H
#define LOCATION_H

#include "ActiveRecord.h"
#include <QAbstractListModel>

class Location;

class LocationCollection : public QList<Location>
{
public:
	QStringList toNameList();
};

class Location : public ActiveRecord
{
	ACTIVE_RECORD(Location, LocationCollection)

	enum Type {
		ALL = -1,
		TENT = 0,
		DORM = 1
	};

public:
	QString tableName();
	Location findByNameType(QString name, Location::Type type);
	LocationCollection findAllByType(Location::Type type);
	void validate();

	ACTIVE_RECORD_FIELD_STRING(getName, setName, "name")
	Location::Type getType();
	void setType(Location::Type value);
	QString getReadableName();

protected:
	void init();
};

#endif // LOCATION_H
