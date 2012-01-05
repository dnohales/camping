#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QSqlRecord>
#include <QDateTime>
#include <QVariant>
#include "CampingException.h"

#define ACTIVE_RECORD_FIELD(getter, setter, type, field) \
	type getter() const { return this->record().value(field).value<type>(); } \
	void setter(type value){ this->setFieldValue(field, QVariant(value)); }

#define ACTIVE_RECORD(className, collectionName) \
	public: \
	className (QSqlRecord record, QObject *parent = 0) : ActiveRecord(record, parent) {} \
	className (bool isTemplate = true, QObject *parent = 0) : ActiveRecord(isTemplate, parent){this->initEmptyRecord();} \
	

class ActiveRecordException : public CampingException
{
public:
	ActiveRecordException(QString message = "", int code = 0): CampingException(message, code) {}
};

/**
 * Una asquerosa implementación del patrón Active record.
 */
class ActiveRecord : public QObject
{
    Q_OBJECT
public:
    ActiveRecord(QSqlRecord record, QObject *parent = 0);
	explicit ActiveRecord(bool isTemplate = true, QObject *parent = 0);
	
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
