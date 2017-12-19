#include "Location.h"
#include <QStringList>

QString Location::tableName()
{
	return "location";
}

void Location::init()
{
	if (this->isNew()) {
		this->setType(Location::TENT);
	}
}

Location Location::findByNameType(QString name, Location::Type type)
{
	return this->find(SqlCriteria()
						  .addCondition("name LIKE :name AND type = :type")
						  .bindValue(":name", name.simplified())
						  .bindValue(":type", type));
}

LocationCollection Location::findAllByType(Location::Type type)
{
	return this->findAll(SqlCriteria()
							 .addCondition("type = :type")
							 .bindValue(":type", type)
							 .setOrder("CAST(name AS INTEGER)"));
}

void Location::validate()
{
	if (this->getName().isEmpty()) {
		throw ActiveRecordException(tr("El nombre de la ubicación no puede estar vacío"));
	}

	Location existentLocation = Location().findByNameType(this->getName(), this->getType());
	if (!existentLocation.isNew() && this->getId() != existentLocation.getId()) {
		throw ActiveRecordException(tr("Ya existe la ubicación"));
	}

	if (this->getType() != Location::TENT && this->getType() != Location::DORM) {
		throw ActiveRecordException(tr("El tipo de ubicación no es válido (%1)").arg(QString::number(this->getType())));
	}
}

Location::Type Location::getType()
{
	return (Location::Type)this->getFieldValue("type").toInt();
}

void Location::setType(Location::Type value)
{
	this->setFieldValue("type", (Location::Type)value);
}

QString Location::getReadableName()
{
	if (this->getType() == Location::DORM) {
		return "Dormi Nº " + this->getName();
	} else {
		return this->getName();
	}
}

QStringList LocationCollection::toNameList()
{
	QStringList list;

	for (int i = 0; i < this->count(); i++) {
		list.append(this->at(i).getName());
	}

	return list;
}
