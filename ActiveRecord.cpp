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

QStringList ActiveRecord::validFieldsList()
{
	QSqlRecord record = this->record();
	QStringList list;
	
	for(int i = 0; i < record.count(); i++){
		QString fieldName = this->record().fieldName(i);
		if(fieldName != "id" && fieldName[0] != '_'){
			list.append(fieldName);
		}
	}
	
	return list;
}

void ActiveRecord::save(bool validate)
{	
	QString q("");
	QSqlQuery query(Db());
	int lastId;
	QStringList fields(validFieldsList());
	
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
		
		for(int i = 0; i < fields.count(); i++){
			q += QString("%1, ").arg(fields[i]);
		}
		q.truncate(q.length() - 2);
		
		q += ") VALUES (";
		for(int i = 0; i < fields.count(); i++){
			q += QString(":%1, ").arg(fields[i]);
		}
		q.truncate(q.length() - 2);
		q += ")";
	} else{
		q += QString("UPDATE %1 SET ").arg(this->tableName());
		
		for(int i = 0; i < fields.count(); i++){
			q += QString("%1 = :%1, ").arg(fields[i]);
		}
		q.truncate(q.length() - 2);
		q += " WHERE id = :id";
	}
	
	query.prepare(q);
	for(int i = 0; i < fields.count(); i++){
		query.bindValue(QString(":")+fields[i], this->record().value(fields[i]));
	}
	if(!this->isNew()){
		query.bindValue(":id", this->getId());
	}
	query.exec();
	printQueryDebug(4, query);
	
	if(query.lastError().isValid()){
		throw ActiveRecordException(QObject::tr("Ocurrió un error al guardar el registro: %1.").arg(query.lastError().text()));
	} else{
		if(!this->isNew()){
			lastId = this->getId();
		} else{
			lastId = query.lastInsertId().toInt();
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

void ActiveRecord::reset()
{
	this->setRecord(this->_originalRecord);
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

QString ActiveRecord::tr(const char *sourceText, const char *comment, int n)
{
	return QObject::tr(sourceText, comment, n);
}


void ActiveRecord::deleteById(int id)
{
	QSqlQuery query = Db().exec("DELETE FROM "+this->tableName()+" WHERE id = "+QString::number(id));
	printQueryDebug(3, query);
}

void ActiveRecord::deleteRecord()
{
	this->deleteById(this->getId());
}

void ActiveRecord::printQueryDebug(int type, QSqlQuery &query)
{
	QString msg;
	QMapIterator<QString, QVariant> i(query.boundValues());
	
	switch(type){
	case 1:
		qDebug() << "[AR: Finding single]" << query.lastQuery();
		break;
	case 2:
		qDebug() << "[AR: Finding collection]" << query.lastQuery();
		break;
	case 3:
		qDebug() << "[AR: Delete]" << query.lastQuery();
		break;
	case 4:
		qDebug() << "[AR: Update]" << query.lastQuery();
		break;
	default:
		qDebug() << "[AR: Other]" << query.lastQuery();
		break;
	}
	
	msg = "";
	while(i.hasNext()){
		i.next();
		msg += i.key() + " => " + i.value().toString() + "\n          ";
	}
	
	if(!msg.isEmpty()){
		qDebug("%s %s", "  [BINDS]", qPrintable(msg.trimmed()));
	}
	
	if(query.lastError().isValid()){
		qDebug() << "  [ERROR]" << query.lastError().text();
	}
}


