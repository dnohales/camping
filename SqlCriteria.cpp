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

QString SqlCriteria::buildSelectQuery()
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
	
	return q;
}

