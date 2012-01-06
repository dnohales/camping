#ifndef LOCATION_H
#define LOCATION_H

#include "ActiveRecord.h"

class Location;

class LocationCollection : public QList<Location>
{
};

class Location : public ActiveRecord
{
	ACTIVE_RECORD(Location, LocationCollection)
	
	enum LocationType {
		TENT = 0,
		DORMI = 1
	};
	
public:
	QString tableName();
	
	ACTIVE_RECORD_FIELD(getName, setName, QString, "name")
	ACTIVE_RECORD_FIELD(getType, setType, int, "type")

protected:
	void init();
};

#endif // LOCATION_H
