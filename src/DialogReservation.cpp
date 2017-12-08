#include "DialogReservation.h"
#include "ui_DialogReservation.h"
#include <QCompleter>
#include <QMessageBox>
#include <QTimer>

DialogReservation::DialogReservation(Reservation *_reservation, Location::Type _type, QWidget *parent)
	: QDialog(parent),
	  completedClient(_reservation->getClient()),
	  reservation(_reservation),
	  type(_type),
	  ui(new Ui::DialogReservation)
{
	ui->setupUi(this);

	this->ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Aceptar"));
	this->ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancelar"));
	this->ui->buttonBox->button(QDialogButtonBox::Reset)->setText(tr("Revertir"));

	if (reservation->isNew()) {
		ui->editName->setFocus();
		this->setWindowTitle(tr("Creando una reservación nueva"));
	} else {
		ui->editDateIn->setFocus();
		this->setWindowTitle(tr("Editando a %1 para el %2").arg(this->completedClient.getFullName(), this->reservation->getDateIn().toString("dd/MM/yyyy")));
	}

	QCompleter *clientCompleter = new QCompleter(new ClientCompleterModel(Client().findAll()));
	clientCompleter->setCaseSensitivity(Qt::CaseInsensitive);
	clientCompleter->setFilterMode(Qt::MatchContains);
	connect(clientCompleter, SIGNAL(activated(QModelIndex)), this, SLOT(onClientCompleterActivated(QModelIndex)));
	this->ui->editName->setCompleter(clientCompleter);

	if (this->type == Location::DORM) {
		ui->labelLocation->setText(tr("Número de dormi"));
		ui->labelTentNum->hide();
		ui->spinTentNum->hide();
	}

	QCompleter *locationCompleter = new QCompleter(Location().findAllByType(this->type).toNameList());
	locationCompleter->setCaseSensitivity(Qt::CaseInsensitive);
	locationCompleter->setFilterMode(Qt::MatchContains);
	this->ui->editLocation->setCompleter(locationCompleter);

	ui->vehicles->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Modelo")));
	ui->vehicles->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Patente")));
	ui->vehicles->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Tamaño")));

	ui->labelLocationNote->hide();

	this->refreshWidgets();
}

DialogReservation::~DialogReservation()
{
	delete ui;
}

void DialogReservation::updateOutDate(int days)
{
	QDate dateIn(this->ui->editDateIn->date());
	QDate dateOut(dateIn.addDays(days));
	this->ui->editDateOut->setDate(dateOut);
}

void DialogReservation::updateDaysCount()
{
	QDateTime dateIn(this->ui->editDateIn->date());
	QDateTime dateOut(this->ui->editDateOut->date());
	int days = dateIn.daysTo(dateOut);

	if (days < 0) {
		this->ui->editDateIn->setStyleSheet("color:red");
		this->ui->editDateOut->setStyleSheet("color:red");
		this->ui->spinDaysIn->setValue(0);
	} else {
		this->ui->editDateIn->setStyleSheet("color:black");
		this->ui->editDateOut->setStyleSheet("color:black");
		this->ui->spinDaysIn->setValue(days);
	}
}

void DialogReservation::onClientCompleterActivated(const QModelIndex &index)
{
	completedClient = Client().findById(index.data(Qt::UserRole).toInt());
	QTimer::singleShot(10, this, SLOT(refreshClientWidgets()));
	ui->editDateIn->setFocus();
}

void DialogReservation::refreshClientWidgets()
{
	if (!completedClient.isNew()) {
		ui->completedClientNotice->show();
		ui->labelCompletedClientNotice->setText(
			tr("Todos los cambios en la información de contacto de <strong>%1</strong> (como el nombre, dirección, teléfono, etc), "
			   "surtirá efecto en el resto de las reservas a nombre de <strong>%1</strong>.")
				.arg(completedClient.getFullName()));
		if (completedClient.getId() != reservation->getClient().getId()) {
			ui->buttonCompletedClientReset->show();
		} else {
			ui->buttonCompletedClientReset->hide();
		}
	} else {
		ui->completedClientNotice->hide();
	}

	this->ui->editName->setText(completedClient.getName());
	this->ui->editSurname->setText(completedClient.getSurame());
	this->ui->editAddress->setText(completedClient.getAdress());
	this->ui->editDni->setText(completedClient.getDni());
	this->ui->editEmail->setText(completedClient.getEmail());
	this->ui->editTel->setText(completedClient.getTel());
}

void DialogReservation::refreshWidgets()
{
	refreshClientWidgets();

	this->ui->editDateIn->setDate(reservation->getDateIn());
	this->ui->editDateOut->setDate(reservation->getDateOut());

	this->ui->editLocation->setText(reservation->getLocation().getName());
	this->ui->spinPeopleNum->setValue(reservation->getPeopleNum());
	this->ui->spinTentNum->setValue(reservation->getTentNum());
	this->ui->editBeck->setText(QString::number(reservation->getBeck()));

	ui->vehicles->setRowCount(0);

	if (!reservation->isNew()) {
		VehicleCollection vlist = reservation->getVehicles();

		for (int i = 0; i < vlist.count(); i++) {
			Vehicle v(vlist.at(i));

			ui->vehicles->insertRow(i);
			ui->vehicles->setItem(i, 0, new QTableWidgetItem(v.getModel()));
			ui->vehicles->setItem(i, 1, new QTableWidgetItem(v.getPatent()));
			ui->vehicles->setItem(i, 2, new QTableWidgetItem(v.getSize()));
			ui->vehicles->item(i, 0)->setData(Qt::UserRole, v.getId());
		}
	}
}

void DialogReservation::onButtonBoxClicked(QAbstractButton *button)
{
	switch (this->ui->buttonBox->buttonRole(button)) {
	case QDialogButtonBox::ResetRole:
		this->reset();
		break;
	default:
		break;
	}
}

void DialogReservation::accept()
{
	ReservationCollection conflicts;
	QString conflictMsg;

	try {
		Db().transaction();

		completedClient.setName(this->ui->editName->text());
		completedClient.setSurame(this->ui->editSurname->text());
		completedClient.setAddress(this->ui->editAddress->text());
		completedClient.setDni(this->ui->editDni->text());
		completedClient.setEmail(this->ui->editEmail->text());
		completedClient.setTel(this->ui->editTel->text());

		Location location = Location().findByNameType(this->ui->editLocation->text(), this->type);
		if (location.isNew()) {
			location.setName(this->ui->editLocation->text());
			location.setType(this->type);
		}

		reservation->setDateIn(this->ui->editDateIn->date());
		reservation->setDateOut(this->ui->editDateOut->date());
		reservation->setPeopleNum(this->ui->spinPeopleNum->value());
		reservation->setTentNum(this->ui->spinTentNum->value());
		reservation->setBeck(this->ui->editBeck->text().toDouble());

		location.validate();
		completedClient.validate();
		reservation->validate();

		if (location.isNew()) {
			location.save();
		}
		reservation->setLocation(location);

		conflicts = reservation->getConflictingReservations();
		if (conflicts.count() > 0) {
			conflictMsg = tr("Ya hay reservationes ocupando la ubicación <b>%1</b> entre las fechas estipuladas, las reservaciones son:<br /><br />").arg(location.getName());
			for (int i = 0; i < conflicts.count(); i++) {
				Reservation conflictReservation(conflicts.at(i));
				conflictMsg += tr("<b>%1</b>: del %2 al %3<br />").arg(conflictReservation.getClient().getFullName(), conflictReservation.getDateIn().toString("d 'de' MMMM"), conflictReservation.getDateOut().toString("d 'de' MMMM"));
			}
			conflictMsg += tr("<br />¿Desea continuar de todas formas?");
			QMessageBox conflictDialog(QMessageBox::Question, tr("Conflictos entre reservaciones"), conflictMsg, QMessageBox::Yes | QMessageBox::No);
			conflictDialog.setTextFormat(Qt::RichText);
			if (conflictDialog.exec() == QMessageBox::No) {
				Db().rollback();
				return;
			}
		}

		completedClient.save();
		reservation->setClient(completedClient);
		reservation->save();

		for (int i = 0; i < ui->vehicles->rowCount(); i++) {
			int id = ui->vehicles->item(i, 0)->data(Qt::UserRole).toInt();
			if ((id == 0 && !this->isVehicleRowEmpty(i)) || id > 0) {
				Vehicle newV(false);
				if (id > 0) {
					newV = Vehicle().findById(id);
				}
				newV.setReservation(*reservation);
				newV.setModel(ui->vehicles->item(i, 0)->text());
				newV.setPatent(ui->vehicles->item(i, 1)->text());
				newV.setSize(ui->vehicles->item(i, 2)->text());

				if (newV.isModified()) {
					newV.save();
				}
			} else if (id < 0) {
				Vehicle().deleteById(-id);
			}
		}

		Db().commit();
		QDialog::accept();
	} catch (ActiveRecordException &e) {
		QMessageBox::critical(this, tr("Error"), e.message());
		Db().rollback();
	}
}

void DialogReservation::reset()
{
	if (QMessageBox::question(
			this,
			tr("Revirtiendo cambios"),
			tr("¿Estás seguro que deseas descartar los cambios hechos y editar la reservación nuevamente?"),
			QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
		this->reservation->reset();
		this->completedClient = this->reservation->getClient();
		this->refreshWidgets();
		this->ui->editName->setFocus();
	}
}

bool DialogReservation::isVehicleRowEmpty(int row)
{
	for (int i = 0; i < ui->vehicles->columnCount(); i++) {
		if (!ui->vehicles->item(row, i)->text().trimmed().isEmpty()) {
			return false;
		}
	}

	return true;
}

void DialogReservation::on_buttonVehicleAdd_clicked()
{
	int lastRowItem = ui->vehicles->rowCount();

	ui->vehicles->insertRow(lastRowItem);
	for (int j = 0; j < ui->vehicles->columnCount(); j++) {
		ui->vehicles->setItem(lastRowItem, j, new QTableWidgetItem(""));
		ui->vehicles->item(lastRowItem, j)->setForeground(QBrush(Qt::darkGreen));
	}

	ui->vehicles->item(lastRowItem, 0)->setData(Qt::UserRole, 0);
}

void DialogReservation::on_buttonVehicleDelete_clicked()
{
	QTableWidget *v = ui->vehicles;
	QFont delFont;
	delFont.setStrikeOut(true);

	for (int i = v->rowCount() - 1; i >= 0; i--) {
		if (v->item(i, 0)->isSelected()) {
			int id = v->item(i, 0)->data(Qt::UserRole).toInt();

			if (id == 0) {
				v->removeRow(i);
			} else {
				v->item(i, 0)->setData(Qt::UserRole, -id);
				for (int j = 0; j < v->columnCount(); j++) {
					v->item(i, j)->setFont(id > 0 ? delFont : QFont());
				}
			}
		}
	}
}

void DialogReservation::on_editLocation_textChanged(QString)
{
	if (Location().findByNameType(ui->editLocation->text(), this->type).isNew()) {
		ui->labelLocationNote->show();
	} else {
		ui->labelLocationNote->hide();
	}
}

void DialogReservation::on_buttonCompletedClientReset_clicked()
{
	completedClient = reservation->getClient();
	refreshClientWidgets();
}
