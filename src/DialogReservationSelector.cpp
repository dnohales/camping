#include "DialogReservationSelector.h"
#include "Reservation.h"
#include "ui_DialogReservationSelector.h"
#include <QMessageBox>
#include <QPushButton>

DialogReservationSelector::DialogReservationSelector(QString idList, QWidget *parent)
	: QDialog(parent),
	  ui(new Ui::DialogReservationSelector)
{
	ui->setupUi(this);

	this->ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Aceptar"));
	this->ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancelar"));

	QStringList ids(idList.split(','));
	qDebug() << ids;

	for (int i = 0; i < ids.count(); i++) {
		Reservation r(Reservation().findById(ids.at(i).toInt()));
		QListWidgetItem *item = new QListWidgetItem();
		item->setText(r.getClient().getFullName());
		item->setData(Qt::UserRole, r.getId());

		ui->list->addItem(item);
	}
}

DialogReservationSelector::~DialogReservationSelector()
{
	delete ui;
}

int DialogReservationSelector::selectedId()
{
	return this->_selectedId;
}

int DialogReservationSelector::exec()
{
	if (ui->list->count() == 1) {
		ui->list->item(0)->setSelected(true);
		this->accept();
		return QDialog::Accepted;
	} else {
		return QDialog::exec();
	}
}

void DialogReservationSelector::accept()
{
	if (ui->list->selectedItems().count() == 0) {
		QMessageBox::warning(this, tr("Error"), tr("Por favor seleccione un cliente."));
	} else {
		this->_selectedId = ui->list->selectedItems().at(0)->data(Qt::UserRole).toInt();
		QDialog::accept();
	}
}

void DialogReservationSelector::on_list_itemActivated(QListWidgetItem *item)
{
	item->setSelected(true);
	this->accept();
}
