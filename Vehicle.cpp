#include "Vehicle.h"

QString Vehicle::tableName()
{
	return "vehicle";
}

QString VehicleCollection::toHtml() const
{
	QString html;
	
	for(int i = 0; i < this->count(); i++){
		html += this->at(i).getModel() + " (" + this->at(i).getPatent() + ")<br />";
	}
	
	return html;
}

QString VehicleCollection::toString() const
{
	QString result;
	
	for(int i = 0; i < this->count(); i++){
		result += this->at(i).getModel() + " (" + this->at(i).getPatent() + ")\n";
	}
	
	return result.trimmed();
}
