#ifndef LOCATION_H
#define LOCATION_H

#include "ActiveRecord.h"

class Location : public ActiveRecord
{
    Q_OBJECT
	ACTIVE_RECORD(Location, LocationCollection)

public:
	QString tableName();
	
	ACTIVE_RECORD_FIELD(getName, setName, QString, "name")
};

#endif // LOCATION_H
