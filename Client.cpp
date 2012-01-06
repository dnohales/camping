#include "Client.h"

QString Client::tableName()
{
	return "client";
}

void Client::init()
{
	if(this->isNew()){
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

QString Client::getFullName()
{
	return this->getName()+" "+this->getSurame();
}

Location Client::getLocation()
{
	return Location().findById(this->getLocationId());
}

void Client::setLocation(const Location &loc)
{
	this->setLocationId(loc.getId());
}

