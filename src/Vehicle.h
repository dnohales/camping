#ifndef VEHICLE_H
#define VEHICLE_H

#include "ActiveRecord.h"
#include "Reservation.h"

#include <QList>

class Vehicle;

class VehicleCollection : public QList<Vehicle>
{
public:
	QString toHtml() const;
	QString toString(const QString &separator = "\n") const;
};

class Vehicle : public ActiveRecord
{
	ACTIVE_RECORD(Vehicle, VehicleCollection)

public:
	QString tableName();

	ACTIVE_RECORD_MANY_TO_ONE(getReservation, setReservation, Reservation, "reservation_id")
	ACTIVE_RECORD_FIELD_STRING(getModel, setModel, "model")
	ACTIVE_RECORD_FIELD_STRING(getPatent, setPatent, "patent")
	ACTIVE_RECORD_FIELD_STRING(getSize, setSize, "size")
};

#endif // VEHICLE_H
