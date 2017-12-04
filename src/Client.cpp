#include "Client.h"
#include <QFile>
#include <QTextCodec>

QString Client::tableName()
{
	return "client";
}

void Client::validate()
{
	if (this->getName().isEmpty() && this->getSurame().isEmpty()) {
		throw ActiveRecordException(tr("Debe especificarse el nombre o apellido del cliente"));
	}
}

QString Client::getFullName() const
{
	return this->getName() + " " + this->getSurame();
}
