#include "Location.h"

QString Location::tableName()
{
	return "location";
}

void Location::init()
{
	if(this->isNew()){
		this->setType(Location::TENT);
	}
}
