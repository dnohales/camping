#include "FrameClients.h"
#include "DialogClient.h"
#include "ui_FrameClients.h"
#include <QMessageBox>

FrameClients::FrameClients(QWidget *parent)
	: MainFrame(parent),
	  ui(new Ui::FrameClients)
{
	ui->setupUi(this);
	resizeEvent(NULL);

	ui->list->addAction(ui->actionListEdit);
	ui->list->addAction(ui->actionListDelete);
}

FrameClients::~FrameClients()
{
	delete ui;
}

SqlCriteria FrameClients::createCriteria()
{
	SqlCriteria criteria;
	QString query;

	if (!mainParent()->searchQuery().isEmpty()) {
		query = "%" + mainParent()->searchQuery() + "%";
	}

	criteria.setSelect("client.*");

	if (!query.isEmpty()) {
		criteria.addCondition("client.name LIKE :query1 OR "
							  "client.surname LIKE :query2 OR "
							  "(client.name||' '||client.surname) LIKE :query3 OR "
							  "client.email LIKE :query4 OR "
							  "client.tel LIKE :query5 OR "
							  "client.dni LIKE :query6 OR "
							  "client.address LIKE :query7");
		criteria.bindValue(":query1", query);
		criteria.bindValue(":query2", query);
		criteria.bindValue(":query3", query);
		criteria.bindValue(":query4", query);
		criteria.bindValue(":query5", query);
		criteria.bindValue(":query6", query);
		criteria.bindValue(":query7", query);
	}

	return criteria;
}

void FrameClients::showClientDialog(Client &c)
{
	DialogClient dialog(&c);
	dialog.exec();
	if (dialog.result() == DialogClient::Accepted) {
		requestRefresh();
	}
}

void FrameClients::onAddClicked()
{
	Client c(false);
	showClientDialog(c);
}

void FrameClients::refreshData()
{
	SqlCriteria criteria = createCriteria();
	ClientCollection currentList = Client().findAll(criteria);

	ui->list->clear();

	for (int i = 0; i < currentList.count(); i++) {
		Client c(currentList.at(i));
		QTreeWidgetItem *item = new QTreeWidgetItem(ui->list);

		item->setText(0, c.getFullName());
		item->setText(1, QString::number(c.getReservations().count()));
		item->setText(2, c.getDni());
		item->setText(3, c.getEmail());
		item->setText(4, c.getTel());
		item->setText(5, c.getAdress());
		item->setText(6, c.getCreateTime().toString("dd/MM/yyyy hh:mm"));

		item->setData(0, Qt::UserRole, c.getId());
	}

	ui->list->header()->resizeSections(QHeaderView::ResizeToContents);

	MainFrame::refreshData();
}

void FrameClients::on_actionListEdit_triggered()
{
	QList<QTreeWidgetItem *> sel = ui->list->selectedItems();
	if (sel.count() > 0) {
		Client c(Client().findById(sel.at(0)->data(0, Qt::UserRole).toInt()));
		showClientDialog(c);
	}
}

void FrameClients::on_actionListDelete_triggered()
{
	QList<QTreeWidgetItem *> sel = ui->list->selectedItems();
	if (sel.count() > 0) {
		Client c(Client().findById(sel.at(0)->data(0, Qt::UserRole).toInt()));
		if (c.getReservations().count() > 0) {
			QMessageBox::critical(
				this,
				tr("Borrando cliente"),
				tr("No se puede borrar a %1 ya que hay reservaciones a su nombre, borre las reservaciones primero").arg(c.getFullName()));
		} else {
			if (QMessageBox::question(
					this,
					tr("Borrando cliente"),
					tr("¿Estás seguro que quieres borrar al cliente %1?").arg(c.getFullName()),
					QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
				c.deleteRecord();
				requestRefresh();
			}
		}
	}
}

void FrameClients::on_list_itemActivated(QTreeWidgetItem *item, int /* column */)
{
	Client c(Client().findById(item->data(0, Qt::UserRole).toInt()));
	showClientDialog(c);
}
