#include "Client.h"

QString Client::tableName()
{
	return "client";
}

void Client::init()
{
	if(this->isNew()){
		this->setDateIn(QDate::currentDate());
		this->setDateOut(QDate::currentDate().addDays(1));
		this->setPeopleNum(0);
		this->setTentNum(0);
	}
}

void Client::validate()
{	
	if(this->getName().isEmpty() && this->getSurame().isEmpty()){
		throw ActiveRecordException(tr("Debe especificarse el nombre o apellido del cliente"));
	}
	
	if(this->getDateOut() < this->getDateIn()){
		throw ActiveRecordException(tr("La fecha de salida no puede ser menor que la de entrada"));
	}
}

QString Client::getFullName() const
{
	return this->getName()+" "+this->getSurame();
}

Location Client::getLocation() const
{
	return Location().findById(this->getLocationId());
}

void Client::setLocation(const Location &loc)
{
	this->setLocationId(loc.getId());
}

int Client::getHousingDays() const
{
	QDateTime dateIn(this->getDateIn());
	QDateTime dateOut(this->getDateOut());
	return dateIn.daysTo(dateOut);
}

bool Client::isHousing() const
{
	return this->getDateOut() > QDate::currentDate();
}

VehicleCollection Client::getVehicles()
{
	return Vehicle().findAll(SqlCriteria().addCondition("client_id = "+QString::number(this->getId())));
}

ClientCollection Client::getConflictingClients()
{
	SqlCriteria criteria;
	criteria.addCondition("id != :id AND location_id = :location_id AND (in_time BETWEEN :di1 AND :do OR (in_time <= :di2 AND out_time > :di3))");
	criteria.bindValue(":id", this->getId());
	criteria.bindValue(":location_id", this->getLocationId());
	criteria.bindValue(":di1", this->getDateIn());
	criteria.bindValue(":do", this->getDateOut().addDays(-1));
	criteria.bindValue(":di2", this->getDateIn());
	criteria.bindValue(":di3", this->getDateIn());
	
	return Client().findAll(criteria);
}

QList<int> ClientCollection::findByLocationAndDate(const Location &loc, const QDate &date)
{
	QList<int> indexList;
	
	for(int i = 0; i < this->count(); i++){
		Client c(this->at(i));
		if(c.getLocationId() == loc.getId() && c.getDateIn() <= date && c.getDateOut() > date){
			indexList.append(i);
		}
	}
	
	return indexList;
}

