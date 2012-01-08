#include "FrameTents.h"
#include "ui_FrameTents.h"
#include "DialogClient.h"
#include "Client.h"
#include "main.h"
#include <QDebug>
#include <QMessageBox>

FrameTents::FrameTents(QWidget *parent) :
    MainFrame(parent),
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
	Client c(false);
	DialogClient dialog(&c, Location::TENT);
	dialog.exec();
	
	if(dialog.result() == DialogClient::Accepted){
		this->requestRefresh();
	}
}

void FrameTents::refreshData()
{	
	SqlCriteria criteria = this->baseCriteria();
	criteria.setOrder("out_time DESC");
	if( !this->ui->checkBoxDorms->isChecked() ){
		criteria.setSelect("client.*, location.type AS _location_type");
		criteria.setJoin("JOIN location ON client.location_id = location.id");
		criteria.addCondition("_location_type = :loctype");
		criteria.bindValue(":loctype", Location::TENT);
	}
	
	ClientCollection col;
	col = Client().findAll(criteria);
	QListIterator<Client> i(col);
	
	this->ui->list->clear();
	
	while(i.hasNext()){
		Client c = i.next();
		Location l = c.getLocation();
		if( this->ui->comboClientStatus->currentIndex() == 0 ||
		   (this->ui->comboClientStatus->currentIndex() == 1 && c.isHousing()) ||
		   (this->ui->comboClientStatus->currentIndex() == 2 && !c.isHousing())
		){
			QString tooltip;
			QTreeWidgetItem *item = new QTreeWidgetItem(this->ui->list);
			
			item->setText(0, c.getFullName());
			item->setText(1, c.getDateIn().toString("dd/MM/yyyy"));
			item->setText(2, c.getDateOut().toString("dd/MM/yyyy") + " (" + QString::number(c.getHousingDays()) + tr(" días)"));
			item->setText(3, l.getName());
			item->setText(4, QString::number(c.getPeopleNum()) + "/" + QString::number(c.getTentNum()));
			
			tooltip = tr("<b>")+c.getFullName()+tr("</b><br /><br />")
			        + tr("<b>DNI: </b>")+c.getDni()+tr("<br />")
			        + tr("<b>Teléfono: </b>")+c.getTel()+tr("<br />")
			        + tr("<b>Celular: </b>")+c.getCel()+tr("<br />")
			        + tr("<b>E-Mail: </b>")+c.getEmail()+tr("<br />")
			        + tr("<b>Dirección: </b>")+c.getAdress();
			item->setToolTip(0, tooltip);
			
			item->setData(0, Qt::UserRole, c.getId());
			
			if(!c.isHousing()){
				for(int j = 0; j < item->columnCount(); j++){
					item->setForeground(j, QBrush(Qt::darkRed));
				}
				item->setData(0, Qt::DecorationRole, QIcon(":/imgs/red-arrow.svg"));
			} else{
				item->setData(0, Qt::DecorationRole, QIcon(":/imgs/green-arrow.svg"));
			}
			
			if(l.getType() == Location::TENT){
				item->setData(3, Qt::DecorationRole, QIcon(":/imgs/tent.png"));
			} else{
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
	if(sel.count() > 0){
		this->on_list_itemActivated(sel.at(0), 0);
	}
}

void FrameTents::on_actionListPrint_triggered()
{
    
}

void FrameTents::on_actionListDelete_triggered()
{
	QList<QTreeWidgetItem *> sel = this->ui->list->selectedItems();
	if(sel.count() > 0){
		Client c(Client().findById(sel.at(0)->data(0, Qt::UserRole).toInt()));
		if(QMessageBox::question(
		            this,
		            tr("Borrando cliente"),
		            tr("¿Estás seguro que quieres borrar a %1?").arg(c.getFullName()),
		            QMessageBox::Yes, QMessageBox::No)
		    == QMessageBox::Yes ){
			c.deleteRecord();
			this->requestRefresh();
		}
	}
}

void FrameTents::on_list_itemActivated(QTreeWidgetItem* item, int /*column*/)
{
    Client c( Client().findById(item->data(0, Qt::UserRole).toInt()) );
	DialogClient dialog(&c, c.getLocation().getType());
	dialog.exec();
	if( dialog.result() == DialogClient::Accepted ){
		this->requestRefresh();
	}
}
