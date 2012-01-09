#include "FrameDorms.h"
#include "ui_FrameDorms.h"
#include "Client.h"
#include "DialogClient.h"
#include "DialogClientSelector.h"
#include <QMenu>
#include <QMessageBox>

FrameDorms::FrameDorms(QWidget *parent) :
    MainFrame(parent),
    ui(new Ui::FrameDorms)
{
    ui->setupUi(this);
}

FrameDorms::~FrameDorms()
{
    delete ui;
}

void FrameDorms::refreshComboMonths()
{
	ClientCollection clist = Client().findAll();
	QList<QDate> totalDates;
	QDate previousSelectedDate;
	bool previousFinded = false;
	
	ui->comboMonth->disconnect(this, SLOT(requestRefresh()));
	
	if(ui->comboMonth->currentIndex() != -1){
		previousSelectedDate = ui->comboMonth->itemData(ui->comboMonth->currentIndex()).toDate();
	} else{
		previousSelectedDate = QDate::currentDate();
		previousSelectedDate.setDate(previousSelectedDate.year(), previousSelectedDate.month(), 1);
	}
	
	for(int i = 0; i < clist.count(); i++){
		QDate date1 = clist.at(i).getDateIn();
		date1.setDate(date1.year(), date1.month(), 1);
		if(!totalDates.contains(date1)) totalDates.append(date1);
		
		QDate date2 = clist.at(i).getDateOut();
		date2.setDate(date2.year(), date2.month(), 1);
		if(!totalDates.contains(date2)) totalDates.append(date2);
	}
	qSort(totalDates);
	
	ui->comboMonth->clear();
	for(int i = 0; i < totalDates.count(); i++){
		ui->comboMonth->addItem(totalDates.at(i).toString("MMMM 'de' yyyy"), totalDates.at(i));

		if(previousSelectedDate == totalDates.at(i)){
			ui->comboMonth->setCurrentIndex(i);
			previousFinded = true;
		}
	}
	if(!previousFinded){
		ui->comboMonth->setCurrentIndex(ui->comboMonth->count()-1);
	}
	
	this->connect(ui->comboMonth, SIGNAL(currentIndexChanged(int)), SLOT(requestRefresh()));
}

void FrameDorms::refreshData()
{
	QDate dateini, dateend;
	
	this->refreshComboMonths();
	
	dateini = ui->comboMonth->itemData(ui->comboMonth->currentIndex()).toDate();
	dateend = ui->comboMonth->itemData(ui->comboMonth->currentIndex()).toDate().addMonths(1).addDays(-1);
	
	//Se busca los clientes que están entre un mes y otro y ocupan dormis
	SqlCriteria criteria;
	criteria.setOrder("in_time");
	criteria.setSelect("client.*, location.type AS _location_type");
	criteria.setJoin("JOIN location ON client.location_id = location.id");
	criteria.addCondition("_location_type = :loctype");
	criteria.bindValue(":loctype", Location::DORM);
	criteria.addCondition("in_time BETWEEN :dateA1 AND :dateA2 OR out_time BETWEEN :dateB1 AND :dateB2");
	criteria.bindValue(":dateA1", dateini);
	criteria.bindValue(":dateA2", dateend);
	criteria.bindValue(":dateB1", dateini);
	criteria.bindValue(":dateB2", dateend);
	
	ClientCollection clist = Client().findAll(criteria);
	LocationCollection dormis = Location().findAllByType(Location::DORM);
	
	if(clist.count() > 0){
		ui->table->setRowCount(dateend.day());
		ui->table->setColumnCount(dormis.count());
		
		for(int i = 0; i < dormis.count(); i++){
			//El encabezado tiene como dato de item el ID del dormi.
			ui->table->setHorizontalHeaderItem(i, new QTableWidgetItem(dormis.at(i).getName()));
			ui->table->horizontalHeaderItem(i)->setData(Qt::UserRole, dormis.at(i).getId());
			
			//Se llenan los datos de un dormi
			for(int day = 0; day < dateend.day(); day++){
				QDate dateToCheck(dateini.addDays(day));
				QList<int> indexList = clist.findByLocationAndDate(dormis.at(i), dateToCheck);
				QTableWidgetItem *item = new QTableWidgetItem();
				QString itemText;
				QString itemData;
				
				//El dato del item puede ser:
				//   0: Está vacío.
				//   <Lista de IDs separados por coma>: Dichos clientes ocupan ese dormi, ese día.
				if(indexList.count() == 0){
					item->setBackgroundColor(qRgb(200,255,200));
					item->setText("");
					item->setData(Qt::UserRole, "0");
				} else{
					item->setBackgroundColor(qRgb(255,200,200));
					itemText = "";
					itemData = "";
					for(int k = 0; k < indexList.count(); k++){
						itemText += clist.at(indexList[k]).getFullName() + "\n";
						itemData += QString::number(clist.at(indexList[k]).getId()) + ",";
					}
					itemData.truncate(itemData.length() - 1);
					item->setText(itemText.trimmed());
					item->setData(Qt::UserRole, itemData);
				}
				ui->table->setItem(day, i, item);
			}
		}
		ui->table->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
		ui->table->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
	}
	
	MainFrame::refreshData();
}

void FrameDorms::on_table_itemActivated(QTableWidgetItem* item)
{
    QString id = item->data(Qt::UserRole).toString();
	QDate date(ui->comboMonth->itemData(ui->comboMonth->currentIndex(), Qt::UserRole).toDate().addDays(item->row()));
	
	if(id == "0"){
		Client c(false);
		c.setLocationId(ui->table->horizontalHeaderItem(item->column())->data(Qt::UserRole).toInt());
		c.setDateIn(date);
		c.setDateOut(date.addDays(1));
		
		DialogClient dialog(&c, Location::DORM);
		dialog.exec();
		if(dialog.result() == DialogClientSelector::Accepted){
			this->requestRefresh();
		}
	} else{
		DialogClientSelector selectorDialog(id);
		selectorDialog.exec();
		if(selectorDialog.result() == DialogClientSelector::Accepted){
			Client c( Client().findById(selectorDialog.selectedId()) );
			DialogClient dialog(&c, Location::DORM);
			dialog.exec();
			if(dialog.result() == DialogClientSelector::Accepted){
				this->requestRefresh();
			}
		}
	}
}

void FrameDorms::on_table_customContextMenuRequested(QPoint pos)
{
	QTableWidgetItem *item = ui->table->itemAt(pos);
	QRect itemRect;
	
	if(item){
		QMenu *menu = new QMenu(this);
		
		if(item->data(Qt::UserRole).toString() == "0"){
			menu->addAction(tr("Crear una reservación aquí"), this, SLOT(onMenuCreate()));
		} else{
			menu->addAction(tr("Editar"), this, SLOT(onMenuEdit()));
			menu->addAction(tr("Imprimir comprobante"), this, SLOT(onMenuPrint()));
			menu->addAction(tr("Borrar"), this, SLOT(onMenuDelete()));
		}
		
		itemRect = ui->table->visualItemRect(item);
		itemRect.moveBottom(itemRect.bottom() + ui->table->horizontalHeader()->height());
		itemRect.moveRight(itemRect.right() + ui->table->verticalHeader()->width());
		menu->popup( ui->table->mapToGlobal(itemRect.bottomLeft()) );
		
		this->selectedItem = item;
	}
}

void FrameDorms::on_buttonAdd_clicked()
{
	Client c(false);
	this->doCreateClient(c, Location::DORM);
}

void FrameDorms::onMenuCreate()
{
	QDate date( ui->comboMonth->itemData(ui->comboMonth->currentIndex(), Qt::UserRole).toDate().addDays(selectedItem->row())  );
	Client c(false);
	c.setLocationId(ui->table->horizontalHeaderItem(selectedItem->column())->data(Qt::UserRole).toInt());
	c.setDateIn(date);
	c.setDateOut(date.addDays(1));
	
	this->doCreateClient(c, Location::DORM);
}

void FrameDorms::onMenuEdit()
{
	DialogClientSelector selectorDialog(selectedItem->data(Qt::UserRole).toString());
	selectorDialog.exec();
	if(selectorDialog.result() == DialogClientSelector::Accepted){
		Client c( Client().findById(selectorDialog.selectedId()) );
		this->doEditClient(c);
	}
}

void FrameDorms::onMenuPrint()
{
	DialogClientSelector selectorDialog( selectedItem->data(Qt::UserRole).toString() );
	selectorDialog.exec();
	if(selectorDialog.result() == DialogClientSelector::Accepted){
		Client c( Client().findById(selectorDialog.selectedId()) );
		this->doPrintReceipt(c);
	}
}

void FrameDorms::onMenuDelete()
{
	DialogClientSelector selectorDialog( selectedItem->data(Qt::UserRole).toString() );
	selectorDialog.exec();
	if(selectorDialog.result() == DialogClientSelector::Accepted){
		Client c( Client().findById(selectorDialog.selectedId()) );
		this->doDeleteClient(c);
	}
}
