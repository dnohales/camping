#include "Vehicle.h"

QString Vehicle::tableName()
{
	return "vehicle";
}

QString VehicleCollection::toHtml()
{
	QString html;
	
	for(int i = 0; i < this->count(); i++){
		html += this->at(i).getModel() + " (" + this->at(i).getPatent() + ")<br />";
	}
	
	return html;
}
