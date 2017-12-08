#include "DialogClient.h"
#include "ui_DialogClient.h"
#include <QMessageBox>
#include <QPushButton>

DialogClient::DialogClient(Client *_client, QWidget *parent)
	: QDialog(parent),
	  client(_client),
	  ui(new Ui::DialogClient)
{
	ui->setupUi(this);

	ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Aceptar"));
	ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancelar"));
	ui->buttonBox->button(QDialogButtonBox::Reset)->setText(tr("Revertir"));

	if (client->isNew()) {
		setWindowTitle(tr("Creando un cliente nuevo"));
	} else {
		setWindowTitle(tr("Editando a %1").arg(client->getFullName()));
	}
	ui->editName->setFocus();

	refreshWidgets();
}

DialogClient::~DialogClient()
{
	delete ui;
}

void DialogClient::refreshWidgets()
{
	ui->editName->setText(client->getName());
	ui->editSurname->setText(client->getSurame());
	ui->editAddress->setText(client->getAdress());
	ui->editDni->setText(client->getDni());
	ui->editEmail->setText(client->getEmail());
	ui->editTel->setText(client->getTel());
}

void DialogClient::onButtonBoxClicked(QAbstractButton *button)
{
	switch (ui->buttonBox->buttonRole(button)) {
	case QDialogButtonBox::ResetRole:
		reset();
		break;
	default:
		break;
	}
}

void DialogClient::accept()
{
	try {
		Db().transaction();

		client->setName(ui->editName->text());
		client->setSurame(ui->editSurname->text());
		client->setAddress(ui->editAddress->text());
		client->setDni(ui->editDni->text());
		client->setEmail(ui->editEmail->text());
		client->setTel(ui->editTel->text());

		client->validate();
		client->save();

		Db().commit();
		QDialog::accept();
	} catch (ActiveRecordException &e) {
		QMessageBox::critical(this, tr("Error"), e.message());
		Db().rollback();
	}
}

void DialogClient::reset()
{
	if (QMessageBox::question(
			this,
			tr("Revirtiendo cambios"),
			tr("¿Estás seguro que deseas descartar los cambios hechos y editar al cliente nuevamente?"),
			QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
		client->reset();
		refreshWidgets();
		ui->editName->setFocus();
	}
}
