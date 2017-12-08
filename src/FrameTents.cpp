#include "FrameTents.h"
#include "DialogReservation.h"
#include "Reservation.h"
#include "main.h"
#include "ui_FrameTents.h"
#include <QDebug>
#include <QMessageBox>
#include <QPrintDialog>

FrameTents::FrameTents(QWidget *parent)
	: FrameReservations(parent),
	  ui(new Ui::FrameTents)
{
	ui->setupUi(this);

	this->ui->list->addAction(this->ui->actionListEdit);
	this->ui->list->addAction(this->ui->actionListPrint);
	this->ui->list->addAction(this->ui->actionListDelete);
}

FrameTents::~FrameTents()
{
	delete ui;
}

void FrameTents::onAddClicked()
{
	Reservation r(false);
	this->doCreateReservation(r, Location::TENT);
}

void FrameTents::refreshData()
{
	SqlCriteria criteria = this->baseCriteria(this->ui->checkBoxDorms->isChecked() ? Location::ALL : Location::TENT);
	criteria.setOrder("in_time DESC");
	_currentList = Reservation().findAll(criteria);

	this->ui->list->clear();

	for (int i = 0; i < _currentList.count(); i++) {
		Reservation r(_currentList.at(i));
		Location l = r.getLocation();
		if (this->ui->comboReservationStatus->currentIndex() == 0 ||
			(this->ui->comboReservationStatus->currentIndex() == 1 && r.isHousing()) ||
			(this->ui->comboReservationStatus->currentIndex() == 2 && !r.isHousing())) {
			QString tooltip;
			QTreeWidgetItem *item = new QTreeWidgetItem(this->ui->list);

			item->setText(0, r.getClient().getFullName());
			item->setText(1, r.getDateIn().toString("dd/MM/yyyy"));
			item->setText(2, r.getDateOut().toString("dd/MM/yyyy") + " (" + QString::number(r.getHousingDays()) + tr(" días)"));
			item->setText(3, l.getName());
			item->setText(4, QString::number(r.getPeopleNum()) + "/" + QString::number(r.getTentNum()));
			item->setText(5, QString::number(r.getBeck()));
			item->setText(6, r.getVehicles().toString(", "));

			tooltip = tr("<b>") + r.getClient().getFullName() + tr("</b><br /><br />") +
					  tr("<b>DNI: </b>") + r.getClient().getDni() + tr("<br />") +
					  tr("<b>Teléfono: </b>") + r.getClient().getTel() + tr("<br />") +
					  tr("<b>E-Mail: </b>") + r.getClient().getEmail() + tr("<br />") +
					  tr("<b>Dirección: </b>") + r.getClient().getAdress();
			item->setToolTip(0, tooltip);

			item->setData(0, Qt::UserRole, r.getId());

			if (!r.isHousing()) {
				for (int j = 0; j < item->columnCount(); j++) {
					item->setForeground(j, QBrush(Qt::darkRed));
				}
				item->setData(0, Qt::DecorationRole, QIcon(":/imgs/red-arrow.svg"));
			} else {
				item->setData(0, Qt::DecorationRole, QIcon(":/imgs/green-arrow.svg"));
			}

			if (l.getType() == Location::TENT) {
				item->setData(3, Qt::DecorationRole, QIcon(":/imgs/tent.png"));
			} else {
				item->setData(3, Qt::DecorationRole, QIcon(":/imgs/dorm.png"));
			}
		}
	}

	this->ui->list->header()->resizeSections(QHeaderView::ResizeToContents);

	MainFrame::refreshData();
}

void FrameTents::on_actionListEdit_triggered()
{
	QList<QTreeWidgetItem *> sel = this->ui->list->selectedItems();
	if (sel.count() > 0) {
		this->on_list_itemActivated(sel.at(0), 0);
	}
}

void FrameTents::on_actionListPrint_triggered()
{
	QList<QTreeWidgetItem *> sel = this->ui->list->selectedItems();
	if (sel.count() > 0) {
		Reservation r(Reservation().findById(sel.at(0)->data(0, Qt::UserRole).toInt()));
		this->doPrintReceipt(r);
	}
}

void FrameTents::on_actionListDelete_triggered()
{
	QList<QTreeWidgetItem *> sel = this->ui->list->selectedItems();
	if (sel.count() > 0) {
		Reservation r(Reservation().findById(sel.at(0)->data(0, Qt::UserRole).toInt()));
		this->doDeleteReservation(r);
	}
}

void FrameTents::on_list_itemActivated(QTreeWidgetItem *item, int /*column*/)
{
	Reservation r(Reservation().findById(item->data(0, Qt::UserRole).toInt()));
	this->doEditReservation(r);
}

ReservationCollection FrameTents::currentList()
{
	return _currentList;
}
