#include "FrameDorms.h"
#include "Reservation.h"
#include "DialogReservation.h"
#include "DialogReservationSelector.h"
#include "ui_FrameDorms.h"
#include <QItemDelegate>
#include <QMenu>
#include <QMessageBox>
#include <QPainter>

class NoBorderItemDelegate : public QItemDelegate
{
public:
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
	{
		QItemDelegate::paint(painter, option, index);
		if (index.data(Qt::UserRole).toString() == "0") {
			painter->setPen(Qt::lightGray);
			painter->drawRect(option.rect);
		}
	}
};

FrameDorms::FrameDorms(QWidget *parent)
	: MainFrame(parent),
	  ui(new Ui::FrameDorms)
{
	ui->setupUi(this);
	this->resizeEvent(NULL);
}

FrameDorms::~FrameDorms()
{
	delete ui;
}

void FrameDorms::refreshComboYears(ReservationCollection &rlist)
{
	QList<QDate> totalDates;
	QDate previousSelectedDate;
	bool previousFinded = false;

	ui->comboYear->disconnect(this, SLOT(requestRefresh()));

	if (ui->comboYear->currentIndex() != -1) {
		previousSelectedDate = ui->comboYear->itemData(ui->comboYear->currentIndex()).toDate();
	} else {
		previousSelectedDate = QDate::currentDate();
		previousSelectedDate.setDate(previousSelectedDate.year(), 1, 1);
	}

	for (int i = 0; i < rlist.count(); i++) {
		QDate date1 = rlist.at(i).getDateIn();
		date1.setDate(date1.year(), 1, 1);
		if (!totalDates.contains(date1))
			totalDates.append(date1);

		QDate date2 = rlist.at(i).getDateOut();
		date2.setDate(date2.year(), 1, 1);
		if (!totalDates.contains(date2))
			totalDates.append(date2);
	}
	qSort(totalDates);

	ui->comboYear->clear();
	for (int i = 0; i < totalDates.count(); i++) {
		ui->comboYear->addItem(totalDates.at(i).toString("yyyy"), totalDates.at(i));

		if (previousSelectedDate == totalDates.at(i)) {
			ui->comboYear->setCurrentIndex(i);
			previousFinded = true;
		}
	}
	if (!previousFinded) {
		ui->comboYear->setCurrentIndex(ui->comboYear->count() - 1);
	}

	this->connect(ui->comboYear, SIGNAL(currentIndexChanged(int)), SLOT(requestRefresh()));
}

void FrameDorms::refreshData()
{
	QDate dateini, dateend;
	ReservationCollection rlist = Reservation().findAll(this->baseCriteria(Location::DORM));
	LocationCollection dormis = Location().findAllByType(Location::DORM);

	//Se refresca el combobox
	this->refreshComboYears(rlist);

	//Por defecto se mostrará de enero a febrero
	dateini = ui->comboYear->itemData(ui->comboYear->currentIndex()).toDate();
	dateend = dateini.addMonths(2).addDays(-1);

	//Se filtra para obtener las reservaciones del año seleccionado
	rlist = rlist.filterByDates(
		dateini,
		dateini.addYears(1).addDays(-1));

	//Recorro las reservaciones filtrados para saber cual será el mes final a mostrar
	for (int i = 0; i < rlist.count(); i++) {
		QDate reservationMaxDate;

		if (rlist.at(i).getDateIn().year() == dateend.year()) {
			reservationMaxDate = rlist.at(i).getDateIn();
		}
		if (rlist.at(i).getDateOut().year() == dateend.year()) {
			reservationMaxDate = rlist.at(i).getDateOut();
		}

		if (reservationMaxDate.month() > dateend.month()) {
			dateend.setDate(dateend.year(), reservationMaxDate.month(), reservationMaxDate.daysInMonth());
		}
	}

	if (rlist.count() > 0) {
		ui->table->setRowCount(dateend.dayOfYear());
		ui->table->setColumnCount(dormis.count());

		for (int i = 0; i < dormis.count(); i++) {
			//El encabezado horizontal tiene como dato de item el ID del dormi.
			ui->table->setHorizontalHeaderItem(i, new QTableWidgetItem(dormis.at(i).getName()));
			ui->table->horizontalHeaderItem(i)->setData(Qt::UserRole, dormis.at(i).getId());
			ui->table->setColumnWidth(i, 130);

			//Se llenan los datos de un dormi
			for (QDate dateToCheck = dateini; dateToCheck <= dateend; dateToCheck = dateToCheck.addDays(1)) {
				ui->table->setVerticalHeaderItem(
					dateToCheck.dayOfYear() - 1,
					new QTableWidgetItem(dateToCheck.toString("d 'de' MMMM")));
				ui->table->verticalHeaderItem(dateToCheck.dayOfYear() - 1)->setData(Qt::UserRole, dateToCheck);

				QList<int> indexList = rlist.findByLocationAndDate(dormis.at(i), dateToCheck);
				QTableWidgetItem *item = new QTableWidgetItem();
				QString itemText;
				QString itemData;

				//El dato del item puede ser:
				//   0: Está vacío.
				//   <Lista de IDs separados por coma>: Dichos clientes ocupan ese dormi, ese día.
				if (indexList.count() == 0) {
					item->setBackgroundColor(qRgb(200, 255, 200));
					item->setText("");
					item->setData(Qt::UserRole, "0");
				} else {
					item->setBackgroundColor(qRgb(255, 200, 200));
					itemText = "";
					itemData = "";
					for (int k = 0; k < indexList.count(); k++) {
						if (dateToCheck == rlist.at(indexList[k]).getDateIn()) {
							itemText += rlist.at(indexList[k]).getClient().getFullName();
							if (rlist.at(indexList[k]).getDateOut().year() > dateToCheck.year()) {
								itemText += QString(" (sigue en %1)").arg(dateToCheck.year() + 1);
							}
							itemText += "\n";
						} else if (dateToCheck.dayOfYear() == 1) {
							itemText += rlist.at(indexList[k]).getClient().getFullName() + QString("(desde %1)\n").arg(dateToCheck.year() - 1);
						}

						itemData += QString::number(rlist.at(indexList[k]).getId()) + ",";
					}
					itemData.truncate(itemData.length() - 1);
					item->setText(itemText.trimmed());
					item->setData(Qt::UserRole, itemData);
				}
				ui->table->setItem(dateToCheck.dayOfYear() - 1, i, item);
			}
		}
	} else {
		ui->table->setRowCount(0);
		ui->table->setColumnCount(0);
	}

	MainFrame::refreshData();
	QTimer::singleShot(10, this, SLOT(onTableColumnWidthUpdate()));
}

void FrameDorms::on_table_itemActivated(QTableWidgetItem *item)
{
	QString id = item->data(Qt::UserRole).toString();
	QDate date(ui->comboYear->itemData(ui->comboYear->currentIndex(), Qt::UserRole).toDate().addDays(item->row()));

	if (id == "0") {
		Reservation r(false);
		r.setLocationId(ui->table->horizontalHeaderItem(item->column())->data(Qt::UserRole).toInt());
		r.setDateIn(date);
		r.setDateOut(date.addDays(1));

		DialogReservation dialog(&r, Location::DORM);
		dialog.exec();
		if (dialog.result() == DialogReservationSelector::Accepted) {
			this->requestRefresh();
		}
	} else {
		DialogReservationSelector selectorDialog(id);
		selectorDialog.exec();
		if (selectorDialog.result() == DialogReservationSelector::Accepted) {
			Reservation r(Reservation().findById(selectorDialog.selectedId()));
			DialogReservation dialog(&r, Location::DORM);
			dialog.exec();
			if (dialog.result() == DialogReservationSelector::Accepted) {
				this->requestRefresh();
			}
		}
	}
}

void FrameDorms::on_table_customContextMenuRequested(QPoint pos)
{
	QTableWidgetItem *item = ui->table->itemAt(pos);
	QRect itemRect;

	if (item) {
		QMenu *menu = new QMenu(this);

		if (item->data(Qt::UserRole).toString() == "0") {
			menu->addAction(tr("Crear una reservación aquí"), this, SLOT(onMenuCreate()));
		} else {
			menu->addAction(tr("Editar"), this, SLOT(onMenuEdit()));
			menu->addAction(tr("Imprimir comprobante"), this, SLOT(onMenuPrint()));
			menu->addAction(tr("Borrar"), this, SLOT(onMenuDelete()));
		}

		itemRect = ui->table->visualItemRect(item);
		itemRect.moveBottom(itemRect.bottom() + ui->table->horizontalHeader()->height());
		itemRect.moveRight(itemRect.right() + ui->table->verticalHeader()->width());
		menu->popup(ui->table->mapToGlobal(itemRect.bottomLeft()));

		this->selectedItem = item;
	}
}

void FrameDorms::on_buttonAdd_clicked()
{
	Reservation r(false);
	this->doCreateReservation(r, Location::DORM);
}

void FrameDorms::onMenuCreate()
{
	QDate date(ui->table->verticalHeaderItem(selectedItem->row())->data(Qt::UserRole).toDate());
	Reservation r(false);
	r.setLocationId(ui->table->horizontalHeaderItem(selectedItem->column())->data(Qt::UserRole).toInt());
	r.setDateIn(date);
	r.setDateOut(date.addDays(1));

	this->doCreateReservation(r, Location::DORM);
}

void FrameDorms::onMenuEdit()
{
	DialogReservationSelector selectorDialog(selectedItem->data(Qt::UserRole).toString());
	selectorDialog.exec();
	if (selectorDialog.result() == DialogReservationSelector::Accepted) {
		Reservation r(Reservation().findById(selectorDialog.selectedId()));
		this->doEditReservation(r);
	}
}

void FrameDorms::onMenuPrint()
{
	DialogReservationSelector selectorDialog(selectedItem->data(Qt::UserRole).toString());
	selectorDialog.exec();
	if (selectorDialog.result() == DialogReservationSelector::Accepted) {
		Reservation r(Reservation().findById(selectorDialog.selectedId()));
		this->doPrintReceipt(r);
	}
}

void FrameDorms::onMenuDelete()
{
	DialogReservationSelector selectorDialog(selectedItem->data(Qt::UserRole).toString());
	selectorDialog.exec();
	if (selectorDialog.result() == DialogReservationSelector::Accepted) {
		Reservation r(Reservation().findById(selectorDialog.selectedId()));
		this->doDeleteReservation(r);
	}
}

void FrameDorms::resizeEvent(QResizeEvent *)
{
	int ccount = ui->table->columnCount();
	int w;

	if (ccount > 0) {
		w = ui->table->viewport()->width() / ccount;

		for (int i = 0; i < ccount; i++) {
			ui->table->setColumnWidth(i, w);
		}
	}
}

void FrameDorms::onTableColumnWidthUpdate()
{
	this->resizeEvent(NULL);
}
