#ifndef SQLCRITERIA_H
#define SQLCRITERIA_H

#include <QString>

class SqlCriteria
{
public:
    explicit SqlCriteria();
	
	SqlCriteria & addCondition(QString condition, QString separator = "AND");
	QString buildSelectQuery();
	
	QString table() { return this->_table; }
	QString where() { return this->_where; }
	QString order() { return this->_order; }
	QString limit() { return this->_limit; }
	QString from() { return this->_from; }
	
	SqlCriteria & setTable(QString value) { this->_table = value; return *this; }
	SqlCriteria & setWhere(QString value) { this->_where = value; return *this; }
	SqlCriteria & setOrder(QString value) { this->_order = value; return *this; }
	SqlCriteria & setLimit(QString value) { this->_limit = value; return *this; }
	SqlCriteria & setFrom(QString value) { this->_from = value; return *this; }

private:
	QString _table;
	QString _where;
	QString _order;
	QString _limit;
	QString _from;
};

#endif // SQLCRITERIA_H
