#include <QMessageBox>
#include <QCompleter>
#include "DialogClient.h"
#include "ui_DialogClient.h"

DialogClient::DialogClient(Client *_client, Location::Type _type, QWidget *parent) :
    QDialog(parent),
    client(_client),
    type(_type),
    ui(new Ui::DialogClient)
{
    ui->setupUi(this);
	
	this->ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Aceptar"));
	this->ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancelar"));
	this->ui->buttonBox->button(QDialogButtonBox::Reset)->setText(tr("Revertir"));
	
	if(client->isNew()){
		this->setWindowTitle(tr("Creando un cliente nuevo"));
	} else{
		this->setWindowTitle(tr("Editando a %1").arg(client->getFullName()));
	}
	
	if(this->type == Location::DORM){
		this->ui->labelLocation->setText(tr("Número de dormi"));
	}
	
	QCompleter *completer = new QCompleter(Location().findAllByType(this->type).toNameList());
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	this->ui->editLocation->setCompleter(completer);
	
	this->refreshWidgets();
}

DialogClient::~DialogClient()
{
    delete ui;
}

void DialogClient::updateOutDate(int days)
{
	QDate dateIn(this->ui->editDateIn->date());
	QDate dateOut(dateIn.addDays(days));
	this->ui->editDateOut->setDate(dateOut);
}

void DialogClient::updateDaysCount()
{
	QDateTime dateIn(this->ui->editDateIn->date());
	QDateTime dateOut(this->ui->editDateOut->date());
	int days = dateIn.daysTo(dateOut);
	
	if(days < 0){
		this->ui->editDateIn->setStyleSheet("color:red");
		this->ui->editDateOut->setStyleSheet("color:red");
		this->ui->spinDaysIn->setValue(0);
	} else{
		this->ui->editDateIn->setStyleSheet("color:black");
		this->ui->editDateOut->setStyleSheet("color:black");
		this->ui->spinDaysIn->setValue(days);
	}
}

void DialogClient::searchLocation()
{
}

void DialogClient::refreshWidgets()
{
	this->ui->editName->setText(client->getName());
	this->ui->editSurname->setText(client->getSurame());
	
	this->ui->editDateIn->setDate(client->getDateIn());
	this->ui->editDateOut->setDate(client->getDateOut());
	
	this->ui->editLocation->setText(client->getLocation().getName());
	this->ui->spinPeopleNum->setValue(client->getPeopleNum());
	this->ui->spinTentNum->setValue(client->getTentNum());
	
	this->ui->editAddress->setText(client->getAdress());
	this->ui->editDni->setText(client->getDni());
	this->ui->editEmail->setText(client->getEmail());
	this->ui->editTel->setText(client->getTel());
	this->ui->editCel->setText(client->getCel());
	
	if(client->isNew()){
		this->ui->editDateIn->setDate(QDate::currentDate());
		this->ui->spinDaysIn->setValue(1);
	}
	
	if(!client->isNew()){
		VehicleCollection vlist = client->getVehicles();
		
		while(ui->vehicles->rowCount() > 0){
			ui->vehicles->removeRow(0);
		}
		
		ui->vehicles->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Modelo")));
		ui->vehicles->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Patente")));
		ui->vehicles->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Tamaño")));
		
		for(int i=0; i < vlist.count(); i++){
			Vehicle v(vlist.at(i));
			
			ui->vehicles->insertRow(i);
			ui->vehicles->setItem(i, 0, new QTableWidgetItem(v.getModel()));
			ui->vehicles->setItem(i, 1, new QTableWidgetItem(v.getPatent()));
			ui->vehicles->setItem(i, 2, new QTableWidgetItem(v.getSize()));
			ui->vehicles->item(i, 0)->setData(Qt::UserRole, v.getId());
		}
	}
}

void DialogClient::onButtonBoxClicked(QAbstractButton *button)
{
	switch(this->ui->buttonBox->buttonRole(button))
	{
	case QDialogButtonBox::ResetRole:
		this->reset();
		break;
	default:
		break;
	}
}

void DialogClient::accept()
{
	try{
		Db().transaction();
		client->setName( this->ui->editName->text() );
		client->setSurame( this->ui->editSurname->text() );
		
		client->setDateIn( this->ui->editDateIn->date() );
		client->setDateOut( this->ui->editDateOut->date() );
		
		client->setPeopleNum( this->ui->spinPeopleNum->value() );
		client->setTentNum( this->ui->spinTentNum->value() );
		
		client->setAddress( this->ui->editAddress->text() );
		client->setDni( this->ui->editDni->text() );
		client->setEmail( this->ui->editEmail->text() );
		client->setTel( this->ui->editTel->text() );
		client->setCel( this->ui->editCel->text() );
		
		client->validate();
		
		Location loc = Location().findByNameType(this->ui->editLocation->text(), this->type);
		if(loc.isNew()){
			loc.setName(this->ui->editLocation->text());
			loc.setType(this->type);
			loc.save();
		}
		client->setLocation(loc);
		client->save();
		
		for(int i=0; i < ui->vehicles->rowCount(); i++){
			int id = ui->vehicles->item(i, 0)->data(Qt::UserRole).toInt();
			if((id == 0 && !this->isVehicleRowEmpty(i)) || id > 0){
				Vehicle newV(false);
				if(id > 0){
					newV = Vehicle().findById(id);
				} else{
					newV.setClientId(client->getId());
				}
				newV.setModel( ui->vehicles->item(i, 0)->text() );
				newV.setPatent( ui->vehicles->item(i, 1)->text() );
				newV.setSize( ui->vehicles->item(i, 2)->text() );
				
				if(newV.isModified()){
					newV.save();
				}
			} else if(id < 0){
				Vehicle().deleteById(-id);
			}
		}
		
		Db().commit();
		QDialog::accept();
	} catch(ActiveRecordException &e){
		QMessageBox::critical(this, tr("Error"), e.message());
		Db().rollback();
	}
}

void DialogClient::reset()
{
	if(QMessageBox::question(
		this,
		tr("Revirtiendo cambios"),
		tr("¿Estás seguro que deseas descartar los cambios hechos y editar el cliente nuevamente?"),
		QMessageBox::Yes, QMessageBox::No
	) == QMessageBox::Yes){
		this->client->reset();
		this->refreshWidgets();
		this->ui->editName->setFocus();
	}
}

bool DialogClient::isVehicleRowEmpty(int row)
{
	for(int i = 0; i < ui->vehicles->columnCount(); i++){
		if(!ui->vehicles->item(row, i)->text().trimmed().isEmpty()){
			return false;
		}
	}
	
	return true;
}

void DialogClient::on_buttonVehicleAdd_clicked()
{
	int lastRowItem = ui->vehicles->rowCount();
    
	ui->vehicles->insertRow(lastRowItem);
	for(int j = 0; j < ui->vehicles->columnCount(); j++){
		ui->vehicles->setItem(lastRowItem, j, new QTableWidgetItem(""));
		ui->vehicles->item(lastRowItem, j)->setForeground(QBrush(Qt::darkGreen));
	}
	
	ui->vehicles->item(lastRowItem, 0)->setData(Qt::UserRole, 0);
}

void DialogClient::on_buttonVehicleDelete_clicked()
{
	QTableWidget *v = ui->vehicles;
	QFont delFont;
	delFont.setStrikeOut(true);
	
	for(int i = v->rowCount()-1; i >= 0; i--){
		if( v->item(i, 0)->isSelected() ){
			int id = v->item(i, 0)->data(Qt::UserRole).toInt();
			
			if(id == 0){
				v->removeRow(i);
			} else {
				v->item(i, 0)->setData(Qt::UserRole, -id);
				for(int j = 0; j < v->columnCount(); j++){
					v->item(i, j)->setFont(id > 0? delFont:QFont());
				}
			}
		}
	}
}
