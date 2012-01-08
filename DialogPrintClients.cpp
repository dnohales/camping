#include "DialogPrintClients.h"
#include "ui_DialogPrintClients.h"

DialogPrintClients::DialogPrintClients(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPrintClients)
{
    ui->setupUi(this);
}

DialogPrintClients::~DialogPrintClients()
{
    delete ui;
}
