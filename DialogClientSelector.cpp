#include "DialogClientSelector.h"
#include "Client.h"
#include "ui_DialogClientSelector.h"
#include <QMessageBox>
#include <QPushButton>

DialogClientSelector::DialogClientSelector(QString idList, QWidget *parent)
	: QDialog(parent),
	  ui(new Ui::DialogClientSelector)
{
	ui->setupUi(this);

	this->ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Aceptar"));
	this->ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancelar"));

	QStringList ids(idList.split(','));
	qDebug() << ids;

	for (int i = 0; i < ids.count(); i++) {
		Client c(Client().findById(ids.at(i).toInt()));
		QListWidgetItem *item = new QListWidgetItem();
		item->setText(c.getFullName());
		item->setData(Qt::UserRole, c.getId());

		ui->list->addItem(item);
	}
}

DialogClientSelector::~DialogClientSelector()
{
	delete ui;
}

int DialogClientSelector::selectedId()
{
	return this->_selectedId;
}

void DialogClientSelector::exec()
{
	if (ui->list->count() == 1) {
		ui->list->item(0)->setSelected(true);
		this->accept();
	} else {
		QDialog::exec();
	}
}

void DialogClientSelector::accept()
{
	if (ui->list->selectedItems().count() == 0) {
		QMessageBox::warning(this, tr("Error"), tr("Por favor seleccione un cliente."));
	} else {
		this->_selectedId = ui->list->selectedItems().at(0)->data(Qt::UserRole).toInt();
		QDialog::accept();
	}
}

void DialogClientSelector::on_list_itemActivated(QListWidgetItem *item)
{
	item->setSelected(true);
	this->accept();
}
