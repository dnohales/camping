#include "FrameTents.h"
#include "ui_FrameTents.h"
#include "DialogClient.h"
#include "Client.h"
#include <QDebug>
#include <QTreeWidget>

FrameTents::FrameTents(QWidget *parent) :
    MainFrame(parent),
    ui(new Ui::FrameTents)
{
    ui->setupUi(this);
}

FrameTents::~FrameTents()
{
    delete ui;
}

void FrameTents::onAddClicked()
{
	Client c(false);
	DialogClient dialog(&c);
	dialog.exec();
}

void FrameTents::refreshData()
{
	SqlCriteria criteria = this->baseCriteria();
	ClientCollection col;
	col = Client().findAll(criteria);
	QListIterator<Client> i(col);
	
	this->ui->list->clear();
	
	while(i.hasNext()){
		Client c = i.next();
		if( this->ui->comboClientStatus->currentIndex() == 0 ||
		   (this->ui->comboClientStatus->currentIndex() == 1 && c.isHousing()) ||
		   (this->ui->comboClientStatus->currentIndex() == 2 && !c.isHousing())
		){
			QTreeWidgetItem *item = new QTreeWidgetItem(this->ui->list);
			
			item->setText(0, c.getFullName());
			item->setText(1, c.getDateIn().toString("dd/MM/yyyy"));
			item->setText(2, c.getDateOut().toString("dd/MM/yyyy") + " (" + QString::number(c.getHousingDays()) + tr(" días)"));
			item->setText(3, c.getLocation().getName());
			item->setText(4, QString::number(c.getPeopleNum()) + "/" + QString::number(c.getTentNum()));
			
			item->setData(0, Qt::UserRole, c.getId());
			
			if(!c.isHousing()){
				for(int j = 0; j < item->columnCount(); j++){
					item->setForeground(j, QBrush(Qt::darkRed));
				}
				item->setData(0, Qt::DecorationRole, QIcon(":/imgs/red-arrow.svg"));
			} else{
				item->setData(0, Qt::DecorationRole, QIcon(":/imgs/green-arrow.svg"));
			}
		}
	}
	
	this->ui->list->header()->resizeSections(QHeaderView::ResizeToContents);
}

