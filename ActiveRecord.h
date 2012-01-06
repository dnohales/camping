#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QDebug>
#include <QSqlRecord>
#include <QDateTime>
#include <QVariant>
#include <QSqlQuery>
#include "CampingException.h"
#include "SqlCriteria.h"
#include "main.h"

#define ACTIVE_RECORD_FIELD(getter, setter, type, field) \
	type getter() const { return this->record().value(field).value<type>(); } \
	void setter(type value){ this->setFieldValue(field, QVariant(value)); }

#define ACTIVE_RECORD(className, collectionName) \
public: \
	className (QSqlRecord record) : ActiveRecord(record) {this->setRecord(record);} \
	className (bool isTemplate = true) : ActiveRecord(isTemplate){this->initEmptyRecord();} \
	className queryToSingle(QSqlQuery query){ \
		query.next(); \
		className object(query.record()); \
		return object; \
	} \
	collectionName queryToCollection(QSqlQuery query){ \
		collectionName c; \
		while(query.next()){ \
			c.append(className(query.record())); \
		} \
		return c; \
	} \
	className find(SqlCriteria criteria = SqlCriteria()){ \
		criteria.setTable(this->tableName()); \
		return this->queryToSingle( Db().exec(criteria.buildSelectQuery()) ); \
	} \
	collectionName findAll(SqlCriteria criteria = SqlCriteria()){ \
		criteria.setTable(this->tableName()); \
		return this->queryToCollection( Db().exec(criteria.buildSelectQuery()) ); \
	} \
	className findById(uint id){ \
		return this->find(SqlCriteria().addCondition(QString("id = ") + QString::number(id))); \
	}
	

class ActiveRecordException : public CampingException
{
public:
	ActiveRecordException(QString message = "", int code = 0): CampingException(message, code) {}
};

/**
 * Una asquerosa implementación del patrón Active record.
 */
class ActiveRecord
{
public:
    ActiveRecord(QSqlRecord record);
	explicit ActiveRecord(bool isTemplate = true);
	
	QSqlRecord record() const;
	void save(bool validate = true);
	bool isModified() const;
	bool isNew() const;
	bool isTemplate() const;
	virtual void validate();
	bool isValid();
	virtual QString tableName() = 0;
	
	ACTIVE_RECORD_FIELD(getId, setId, uint, "id")
	ACTIVE_RECORD_FIELD(getCreateTime, setCreateTime, QDateTime, "create_time")
	ACTIVE_RECORD_FIELD(getUpdateTime, setUpdateTime, QDateTime, "update_time")
	
protected:
	void initEmptyRecord();
	virtual void init();
	void setRecord(QSqlRecord record);
	QVariant getFieldValue(QString name);
	void setFieldValue(QString name, QVariant value);
	
private:
	QSqlRecord _record;
	QSqlRecord _originalRecord;
	bool _isTemplate;
};

#endif // MODEL_H
