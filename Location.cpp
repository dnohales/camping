#include "Location.h"

QString Location::tableName()
{
	return "location";
}

void Location::init()
{
	if(this->isNew()){
		this->setType(Location::TENT);
	}
}

Location Location::findByNameType(QString name, Location::Type type)
{
	return this->find(SqlCriteria()
	                  .addCondition("name = :name AND type = :type")
	                  .bindValue(":name", name)
	                  .bindValue(":type", type)
	                 );
}

LocationCollection Location::findAllByType(Location::Type type)
{
	return this->findAll(SqlCriteria()
	                  .addCondition("type = :type")
	                  .bindValue(":type", type)
	                  .setOrder("name ASC")
	                     );
}

void Location::validate()
{
	if(this->getName().isEmpty()){
		throw ActiveRecordException(tr("El nombre de la ubicación no puede estar vacío"));
	}
	
	if( !Location().findByNameType(this->getName(), this->getType()).isNew() ){
		throw ActiveRecordException( tr("Ya existe la ubicación") );
	}
	
	if(this->getType() != Location::TENT && this->getType() != Location::DORMI){
		throw ActiveRecordException( tr("El tipo de ubicación no es válido (%1)").arg(QString::number(this->getType())) );
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

int LocationModel::rowCount(const QModelIndex &) const
{
	return collection.count();
}

QVariant LocationModel::data(const QModelIndex &index, int role) const
{
	/*if (!index.isValid())
		return QVariant();
	
	if (index.row() >= collection.size())
		return QVariant();*/
	
	//if (role == Qt::DisplayRole)
		return collection.at(index.row()).getName();
	//else
		//return QVariant();
}

