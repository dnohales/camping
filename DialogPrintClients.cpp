#include "DialogPrintClients.h"
#include "ui_DialogPrintClients.h"
#include <QPushButton>

DialogPrintClients::DialogPrintClients(const ClientCollection &list, QWidget *parent) :
	QDialog(parent),
	originalList(list),
	ui(new Ui::DialogPrintClients)
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

DialogPrintClients::~DialogPrintClients()
{
	delete ui;
}

const ClientCollection * DialogPrintClients::currentList() const
{
	const ClientCollection *c;

	if(ui->checkBoxFilter->isChecked()){
		c = &filteredList;
	} else{
		c = &originalList;
	}

	return c;
}

void DialogPrintClients::updateFilteredList()
{
	this->filteredList = this->originalList.filterByDates(ui->dateIni->date(), ui->dateEnd->date());
	this->updateInfo();
}

void DialogPrintClients::updateInfo()
{
	const ClientCollection *c = this->currentList();
	ui->dateIni->setEnabled(ui->checkBoxFilter->isChecked());
	ui->dateEnd->setEnabled(ui->checkBoxFilter->isChecked());

	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(c->count() > 0);
	ui->labelInfo->setText(tr("Se imprimiran %1 cliente(s)").arg(c->count()));
}

void DialogPrintClients::accept()
{
	App()->printHtml(currentList()->toHtmlDocument(ui->editTitle->text()));
	QDialog::accept();
}
