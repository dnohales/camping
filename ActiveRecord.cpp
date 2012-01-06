#include "main.h"
#include "common.h"
#include "ActiveRecord.h"

ActiveRecord::ActiveRecord(QSqlRecord )
{
}

ActiveRecord::ActiveRecord(bool isTemplate)
{
	this->_isTemplate = isTemplate;
}

QSqlRecord ActiveRecord::record() const
{
	return this->_record;
}

void ActiveRecord::save(bool validate)
{	
	QString q("");
	QSqlQuery query(Db());
	uint lastId;
	
	if(this->isTemplate()){
		throw ActiveRecordException(QObject::tr("Una ActiveRecord plantilla es solo para fabricar otros ActiveRecord's."));
	}
	
	if(validate){
		this->validate();
	}
	
	this->setUpdateTime(QDateTime::currentDateTime());
	
	if(this->isNew()){
		this->setCreateTime(QDateTime::currentDateTime());
		q += QString("INSERT INTO %1 (").arg(this->tableName());
		
		for(int i = 0; i < this->record().count(); i++){
			if(this->record().fieldName(i) != "id"){
				q += QString("%1, ").arg(this->record().fieldName(i));
			}
		}
		q.truncate(q.length() - 2);
		
		q += ") VALUES (";
		for(int i = 0; i < this->record().count(); i++){
			if(this->record().fieldName(i) != "id"){
				q += QString(":%1, ").arg(this->record().fieldName(i));
			}
		}
		q.truncate(q.length() - 2);
		q += ")";
	} else{
		q += QString("UPDATE %1 SET ").arg(this->tableName());
		
		for(int i = 0; i < this->record().count(); i++){
			if(this->record().fieldName(i) != "id"){
				q += QString("%1 = :%1, ").arg(this->record().fieldName(i));
			}
		}
		q.truncate(q.length() - 2);
		q += " WHERE id = "+this->getId();
	}
	
	query.prepare(q);
	for(int i = 0; i < this->record().count(); i++){
		if(this->record().fieldName(i) != "id"){
			query.bindValue(QString(":")+this->record().fieldName(i), this->record().value(i));
		}
	}
	query.exec();
	qDebug() << QObject::tr("Salvando ActiveRecord: Consulta SQL: ") + query.executedQuery();
	
	if(query.lastError().isValid()){
		throw ActiveRecordException(QObject::tr("Ocurrió un error al guardar el registro: %1.").arg(query.lastError().text()));
	} else{
		if(!this->isNew()){
			lastId = this->getId();
		} else{
			lastId = query.lastInsertId().toUInt();
		}
		query.clear();
		query.prepare(QString("SELECT * FROM %1 WHERE id = :id").arg(this->tableName()));
		query.bindValue(":id", lastId);
		query.exec();
		query.next();
		this->setRecord(query.record());
	}
}

void ActiveRecord::init()
{
}

void ActiveRecord::initEmptyRecord()
{
	QSqlQuery query("SELECT * FROM "+this->tableName()+" LIMIT 0", Db());
	this->setRecord(query.record());
}

void ActiveRecord::setRecord(QSqlRecord record)
{
	this->_record = record;
	this->_originalRecord = record;
	this->_isTemplate = false;
	this->init();
}

bool ActiveRecord::isModified() const
{
	return this->_record != this->_originalRecord;
}

bool ActiveRecord::isNew() const
{
	return this->record().isNull("id");
}

bool ActiveRecord::isTemplate() const
{
	return this->_isTemplate;
}

void ActiveRecord::validate()
{
}

bool ActiveRecord::isValid()
{
	try{
		this->validate();
		return true;
	} catch(ActiveRecordException &e){
		return false;
	}
}

QVariant ActiveRecord::getFieldValue(QString name)
{
	return this->_record.value(name);
}

void ActiveRecord::setFieldValue(QString name, QVariant value)
{
	this->_record.setValue(name, value);
}


