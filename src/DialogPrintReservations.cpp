#include "DialogPrintReservations.h"
#include "ui_DialogPrintReservations.h"
#include <QPushButton>

DialogPrintReservations::DialogPrintReservations(const ReservationCollection &list, QWidget *parent)
	: QDialog(parent),
	  originalList(list),
	  ui(new Ui::DialogPrintReservations)
{
	ui->setupUi(this);

	ui->checkBoxFilter->setChecked(false);

	ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancelar"));
	ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Imprimir"));

	QDate dateini(QDate::currentDate());
	ui->dateIni->setDate(dateini);
	ui->dateEnd->setDate(dateini.addDays(1));

	this->updateInfo();
}

DialogPrintReservations::~DialogPrintReservations()
{
	delete ui;
}

const ReservationCollection *DialogPrintReservations::currentList() const
{
	const ReservationCollection *c;

	if (ui->checkBoxFilter->isChecked()) {
		c = &filteredList;
	} else {
		c = &originalList;
	}

	return c;
}

void DialogPrintReservations::updateFilteredList()
{
	this->filteredList = this->originalList.filterByDates(ui->dateIni->date(), ui->dateEnd->date());
	this->updateInfo();
}

void DialogPrintReservations::updateInfo()
{
	const ReservationCollection *c = this->currentList();
	ui->dateIni->setEnabled(ui->checkBoxFilter->isChecked());
	ui->dateEnd->setEnabled(ui->checkBoxFilter->isChecked());

	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(c->count() > 0);
	ui->labelInfo->setText(tr("Se imprimiran %1 reservación(es)").arg(c->count()));
}

void DialogPrintReservations::accept()
{
	App()->printHtml(currentList()->toHtmlDocument(ui->editTitle->text()));
	QDialog::accept();
}
