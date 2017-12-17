#ifndef SQLCRITERIA_H
#define SQLCRITERIA_H

#include <QSqlQuery>
#include <QString>
#include <QVariant>

class SqlCriteria
{
public:
	explicit SqlCriteria();

	SqlCriteria &addCondition(QString condition, QString separator = "AND");
	QSqlQuery buildSelectQuery();
	QString buildSelectQueryAsString();

	SqlCriteria &bindValue(const QString &placeholder, const QVariant &val);

	QString table() { return this->_table; }
	QString where() { return this->_where; }
	QString order() { return this->_order; }
	QString limit() { return this->_limit; }
	QString from() { return this->_from; }
	QString join() { return this->_join; }
	QString select() { return this->_select; }
	QString group() { return this->_group; }

	SqlCriteria &setTable(QString value)
	{
		this->_table = value;
		return *this;
	}
	SqlCriteria &setWhere(QString value)
	{
		this->_where = value;
		return *this;
	}
	SqlCriteria &setOrder(QString value)
	{
		this->_order = value;
		return *this;
	}
	SqlCriteria &setLimit(QString value)
	{
		this->_limit = value;
		return *this;
	}
	SqlCriteria &setFrom(QString value)
	{
		this->_from = value;
		return *this;
	}
	SqlCriteria &setJoin(QString value)
	{
		this->_join = value;
		return *this;
	}
	SqlCriteria &setSelect(QString value)
	{
		this->_select = value;
		return *this;
	}
	SqlCriteria &setGroup(QString value)
	{
		this->_group = value;
		return *this;
	}

private:
	QString _table;
	QString _where;
	QString _order;
	QString _limit;
	QString _from;
	QString _join;
	QString _select;
	QString _group;
	QMap<QString, QVariant> _binds;
};

#endif // SQLCRITERIA_H
