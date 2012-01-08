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

int ClientModel::rowCount(const QModelIndex &) const
{
	return collection.count();
}

int ClientModel::columnCount(const QModelIndex &) const
{
	return 5;
}

QVariant ClientModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		 return QVariant();

	 if (index.row() >= collection.size())
		 return QVariant();

	 if (role == Qt::DisplayRole){
		 Client c(collection.at(index.row()));
		 switch(index.column()){
		 case 0:
			 return c.getFullName();
		 case 1:
			 return c.getDateIn().toString("dd/MM/yyyy");
		 case 2:
			 return c.getDateOut().toString("dd/MM/yyyy");
		 case 3:
			 return c.getLocation().getName();
		 case 4:
			 return QString::number(c.getPeopleNum()) + "/" + QString::number(c.getTentNum());
		 }
	 }
	 else
		 return QVariant();
}

QVariant ClientModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole)
		 return QVariant();

	if (orientation == Qt::Horizontal){
		switch(section){
		case 0:
			return tr("Nombre");
		case 1:
			return tr("Fecha de entrada");
		case 2:
			return tr("Fecha de salida");
		case 3:
			return tr("Ubicación");
		case 4:
			return tr("Cantidad personas/carpas");		
		}
	}
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




