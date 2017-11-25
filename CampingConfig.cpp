#include "main.h"
#include "common.h"
#include <QSettings>

CampingConfig::CampingConfig(QObject *parent) :
	QObject(parent)
{
	this->clear();
}

void CampingConfig::init()
{
	this->clear();

	QSqlQuery query("SELECT key, value FROM config", Db());
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
	QSqlQuery recordQuery(Db());

	Db().transaction();
	Db().exec("DELETE FROM config");

	while(i.hasNext()){
		i.next();

		QString queryStr;
		queryStr = "INSERT INTO config(key, value) VALUES (:key, :value)";

		recordQuery.prepare(queryStr);
		recordQuery.bindValue(":value", i.value());
		recordQuery.bindValue(":key", i.key());
		recordQuery.exec();
	}
	Db().commit();

	this->init();
}

int CampingConfig::dbVersion()
{
	return this->data["db_version"].toInt();
}

void CampingConfig::setDbVersion(int version)
{
	this->data["db_version"] = QVariant(version);
}

QString CampingConfig::lastFilename()
{
	QSettings s;
	return s.value("LastFilename").toString();
}

void CampingConfig::setLastFilename(QString filename)
{
	QSettings s;
	s.setValue("LastFilename", filename);
}

QString CampingConfig::receiptTemplate()
{
	if(data.contains("custom_receipt") && !data["custom_receipt"].toString().isEmpty()){
		return data["custom_receipt"].toString();
	} else{
		QFile htmlFile(":/html/receipt.html");
		htmlFile.open(QFile::ReadOnly);
		return htmlFile.readAll();
	}
}

void CampingConfig::setCustomReceipt(QString receipt)
{
	this->data["custom_receipt"] = QVariant(receipt);
}

void CampingConfig::restoreReceipt()
{
	this->data["custom_receipt"] = QVariant();
}

