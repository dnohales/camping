#ifndef MODEL_H
#define MODEL_H

#include "CampingException.h"
#include "SqlCriteria.h"
#include "main.h"
#include <QDateTime>
#include <QDebug>
#include <QObject>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#define ACTIVE_RECORD_FIELD(getter, setter, type, field)                      \
	type getter() const { return this->record().value(field).value<type>(); } \
	void setter(type value) { this->setFieldValue(field, QVariant(value)); }

#define ACTIVE_RECORD_FIELD_STRING(getter, setter, field)                           \
	QString getter() const { return this->record().value(field).value<QString>(); } \
	void setter(QString value) { this->setFieldValue(field, QVariant(value.simplified())); }

#define ACTIVE_RECORD(className, collectionName)                                               \
public:                                                                                        \
	className(QSqlRecord record) : ActiveRecord(record) { this->setRecord(record); }           \
	className(bool isTemplate = true) : ActiveRecord(isTemplate) { this->initEmptyRecord(); }  \
	className queryToSingle(QSqlQuery query)                                                   \
	{                                                                                          \
		printQueryDebug(1, query);                                                             \
		query.next();                                                                          \
		className object(query.record());                                                      \
		return object;                                                                         \
	}                                                                                          \
	collectionName queryToCollection(QSqlQuery query)                                          \
	{                                                                                          \
		printQueryDebug(2, query);                                                             \
		collectionName c;                                                                      \
		while (query.next()) {                                                                 \
			c.append(className(query.record()));                                               \
		}                                                                                      \
		return c;                                                                              \
	}                                                                                          \
	className find(SqlCriteria criteria = SqlCriteria())                                       \
	{                                                                                          \
		criteria.setTable(this->tableName());                                                  \
		QSqlQuery query(criteria.buildSelectQuery());                                          \
		query.exec();                                                                          \
		return this->queryToSingle(query);                                                     \
	}                                                                                          \
	collectionName findAll(SqlCriteria criteria = SqlCriteria())                               \
	{                                                                                          \
		criteria.setTable(this->tableName());                                                  \
		QSqlQuery query(criteria.buildSelectQuery());                                          \
		query.exec();                                                                          \
		return this->queryToCollection(query);                                                 \
	}                                                                                          \
	className findById(int id)                                                                 \
	{                                                                                          \
		return this->find(SqlCriteria().addCondition(QString("id = ") + QString::number(id))); \
	}

class ActiveRecordException : public CampingException
{
public:
	ActiveRecordException(QString message = "", int code = 0)
		: CampingException(message, code) {}
};

/**
 * Una asquerosa implementación del patrón Active record.
 */
class ActiveRecord
{
public:
	ActiveRecord(QSqlRecord record);
	explicit ActiveRecord(bool isTemplate = true);
	virtual ~ActiveRecord();

	QSqlRecord record() const;
	void save(bool validate = true);
	void reset();
	bool isModified() const;
	bool isNew() const;
	bool isTemplate() const;
	virtual void validate();
	bool isValid();
	virtual QString tableName() = 0;
	void deleteById(int id);
	void deleteRecord();

	static QString tr(const char *sourceText, const char *comment = 0, int n = -1);

	ACTIVE_RECORD_FIELD(getId, setId, int, "id")
	ACTIVE_RECORD_FIELD(getCreateTime, setCreateTime, QDateTime, "create_time")
	ACTIVE_RECORD_FIELD(getUpdateTime, setUpdateTime, QDateTime, "update_time")

protected:
	void initEmptyRecord();
	virtual void init();
	void setRecord(QSqlRecord record);
	QVariant getFieldValue(QString name);
	void setFieldValue(QString name, QVariant value);
	void printQueryDebug(int type, QSqlQuery &query);
	QStringList validFieldsList();

private:
	QSqlRecord _record;
	QSqlRecord _originalRecord;
	bool _isTemplate;
};

#endif // MODEL_H
