#include "main.h"

CampingConfig::CampingConfig(QObject *parent) :
    QObject(parent)
{
	this->clear();
}

void CampingConfig::init()
{
	this->clear();
	
	QSqlQuery query("SELECT key, value FROM config");
	while(query.next()){
		this->data[query.value(0).toString()] = query.value(1);
	}
}

void CampingConfig::clear()
{
	this->data.clear();
	
	this->setDbVersion(App()->dbVersion());
}

void CampingConfig::save()
{
	QMapIterator<QString, QVariant> i(this->data);
	QSqlQuery query;
	QSqlQuery recordQuery;
	
	Db().transaction();
	while(i.hasNext()){
		i.next();

		query.prepare("SELECT * FROM config WHERE key = :key");
		query.bindValue(":key", i.key());
		query.exec();
		
		QString queryStr;
		if(query.size() > 0){
			queryStr = "UPDATE";
		} else{
			queryStr = "INSERT INTO";
		}
		queryStr += " config SET value = :value";
		if(query.size() > 0){
			queryStr += " WHERE key = :key";
		}
		
		recordQuery.prepare(queryStr);
		recordQuery.bindValue(":value", i.value());
		recordQuery.bindValue(":key", i.key());
		recordQuery.exec();
	}
	Db().commit();
}

int CampingConfig::dbVersion()
{
	return this->data["db_version"].toInt();
}

void CampingConfig::setDbVersion(int version)
{
	this->data["db_version"] = QVariant(version);
}

