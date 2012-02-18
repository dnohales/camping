#include "Vehicle.h"
#include <QStringList>

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

QString VehicleCollection::toString(const QString &separator) const
{
	QStringList vlist;
	
	for(int i = 0; i < this->count(); i++){
		vlist.append(this->at(i).getModel() + " (" + this->at(i).getPatent() + ")");
	}
	
	return vlist.join(separator);
}
