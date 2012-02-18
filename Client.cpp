#include "Client.h"
#include <QFile>
#include <QTextCodec>

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

VehicleCollection Client::getVehicles() const
{
	return Vehicle().findAll(SqlCriteria().addCondition("client_id = "+QString::number(this->getId())));
}

ClientCollection Client::getConflictingClients() const
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

QList<int> ClientCollection::findByLocationAndDate(const Location &loc, const QDate &date) const
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

QString Client::getReceiptHtml() const
{
	QString html(App()->config()->receiptTemplate());
	
	html.replace("{nombre}", this->getName(), Qt::CaseInsensitive)
	    .replace("{apellido}", this->getSurame(), Qt::CaseInsensitive)
	    .replace("{dni}", this->getDni(), Qt::CaseInsensitive)
	    .replace("{direccion}", this->getAdress(), Qt::CaseInsensitive)
	    .replace("{cantidad_personas}", QString::number(this->getPeopleNum()), Qt::CaseInsensitive)
		.replace("{vehiculos}", this->getVehicles().toHtml(), Qt::CaseInsensitive)
		.replace("{ubicacion}", this->getLocation().getReadableName(), Qt::CaseInsensitive)
		.replace("{fecha_entrada}", this->getDateIn().toString("dd/MM/yyyy"), Qt::CaseInsensitive)
		.replace("{fecha_salida}", this->getDateOut().toString("dd/MM/yyyy"), Qt::CaseInsensitive);
	
	if(this->getLocation().getType() != Location::DORM){
		html.replace("{cantidad_carpas}", QString::number(this->getTentNum()), Qt::CaseInsensitive);
		html.replace("{cantidad_carpas_style}", "visibility:visible;", Qt::CaseInsensitive);
	} else{
		html.replace("{cantidad_carpas_style}", "visibility:hidden;", Qt::CaseInsensitive);
	}
	
	return html;
}

QString ClientCollection::toHtmlDocument(QString title) const
{
	QFile htmlFile(":/html/client-collection.html");
	htmlFile.open(QFile::ReadOnly);
	QString html(htmlFile.readAll());
	htmlFile.close();
	QString listString;
	QTextStream list(&listString);
	
	list << "<table class=\"datatable\">"
		<< "<tr>"
		<< "<th>Cliente</th>"
		<< "<th>F. Entrada</th>"
		<< "<th>F. Salida</th>"
		<< "<th>Ubicaci&oacute;n</th>"
		<< "<th>Personas/Carpas</th>"
	    << "<th>Veh&iacute;culos</th>"
	    << "<th>Se&nacute;a</th>"
		<< "<th>Tel&eacute;fono</th>"
		<< "</tr>";
	for(int i = 0; i < this->count(); i++){
		Client c(this->at(i));
		list << "<tr>";
		
		list << "<td>" << c.getFullName() << "</td>";
		list << "<td>" << c.getDateIn().toString("dd/MM/yyyy") << "</td>";
		list << "<td>" << c.getDateOut().toString("dd/MM/yyyy") << "</td>";
		list << "<td>" << c.getLocation().getReadableName() << "</td>";
		list << "<td>" << c.getPeopleNum() << "/" << c.getTentNum() << "</td>";
		list << "<td>" << c.getVehicles().toHtml() << "</td>";
		list << "<td>" << c.getBeck() << "</td>";
		list << "<td>" << c.getTel() << "</td>";
		
		list << "</tr>";
	}
	list << "</table>";
	
	html.replace("{titulo}", title)
		.replace("{clientes}", listString)
		.replace("{pie_de_pagina}", QDateTime::currentDateTime().toString("'Impreso el' d 'de' MMMM 'de' yyyy 'a las' hh:mm"));
	
	return html;
}

ClientCollection ClientCollection::filterByDates(const QDate &dateini, const QDate &dateend) const
{
	ClientCollection col;
	
	for(int i = 0; i < this->count(); i++){
		Client c(this->at(i));
		
		if( (c.getDateIn() >= dateini && c.getDateIn() <= dateend) ||
		    (c.getDateOut() >= dateini && c.getDateOut() <= dateend) ){
			col.append(c);
		}
	}
	
	return col;
}


