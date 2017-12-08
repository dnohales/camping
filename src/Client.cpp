#include "Client.h"
#include "Reservation.h"
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

ReservationCollection Client::getReservations() const
{
	return Reservation().findAll(SqlCriteria().addCondition("client_id = " + QString::number(this->getId())));
}

ClientCompleterModel::ClientCompleterModel(const ClientCollection &collection)
	: collection(collection)
{
}

QVariant ClientCompleterModel::data(const QModelIndex &index, int role) const
{
	const Client *c = &this->collection.at(index.row());
	switch (role) {
	case Qt::DisplayRole:
	case Qt::EditRole:
		return QVariant(QString("%1 - %2").arg(c->getFullName(), c->getDni()));
	case Qt::UserRole:
		return QVariant(c->getId());
	default:
		return QVariant();
	}
}

int ClientCompleterModel::rowCount(const QModelIndex &) const
{
	return this->collection.count();
}
