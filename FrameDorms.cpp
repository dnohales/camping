#include "FrameDorms.h"
#include "ui_FrameDorms.h"
#include "Client.h"
#include "DialogClient.h"

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

void FrameDorms::refreshData()
{
	qDebug() << "Refrescando Dorms";
	
	MainFrame::refreshData();
}

void FrameDorms::on_buttonAdd_clicked()
{
	Client c(false);
	DialogClient dialog(&c, Location::DORM);
	dialog.exec();
}
