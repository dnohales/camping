#ifndef VEHICLE_H
#define VEHICLE_H

#include "ActiveRecord.h"
#include <QList>

class Vehicle;

class VehicleCollection : public QList<Vehicle>
{
};

class Vehicle : public ActiveRecord
{
	ACTIVE_RECORD(Vehicle, VehicleCollection)
	
public:
    QString tableName();
	
	ACTIVE_RECORD_FIELD(getModel, setModel, QString, "model")
	ACTIVE_RECORD_FIELD(getPatent, setPatent, QString, "patent")
	ACTIVE_RECORD_FIELD(getSize, setSize, QString, "size")
};

#endif // VEHICLE_H
