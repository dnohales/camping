#include <QMapIterator>
#include <QDebug>
#include "main.h"
#include "SqlCriteria.h"

SqlCriteria::SqlCriteria()
{
}

SqlCriteria & SqlCriteria::addCondition(QString condition, QString separator)
{
	if(!this->where().isEmpty()){
		this->setWhere("(" + this->where() + ") " + separator + " (" + condition + ")");
	} else{
		this->setWhere(condition);
	}
	return *this;
}

QSqlQuery SqlCriteria::buildSelectQuery()
{
	QString q = "SELECT * FROM " + this->table();
	if(!this->where().isEmpty()){
		q += " WHERE " + this->where();
	}
	
	if(!this->order().isEmpty()){
		q += " ORDER BY " + this->order();
	}
	
	if(!this->limit().isEmpty()){
		q += " LIMIT " + this->limit();
		if(!this->order().isEmpty()){
			q += ", " + this->from();
		}
	}
	
	QSqlQuery query(Db());
	QMapIterator<QString, QVariant> i(_binds);
	query.prepare(q);
	while(i.hasNext()){
		i.next();
		query.bindValue(i.key(), i.value());
	}
	return query;
}

SqlCriteria SqlCriteria::bindValue(const QString& placeholder, const QVariant& val)
{
    _binds[placeholder] = val;
	return *this;
}

/*void SqlCriteria::bindValue(int pos, const QVariant& val, QSql::ParamType paramType)
{
    _query.bindValue(pos, val, paramType);
}

void SqlCriteria::addBindValue(const QVariant& val, QSql::ParamType paramType)
{
    _query.addBindValue(val, paramType);
}

QVariant SqlCriteria::boundValue(const QString& placeholder) const
{
    return _query.boundValue(placeholder);
}

QVariant SqlCriteria::boundValue(int pos) const
{
    return _query.boundValue(pos);
}*/

