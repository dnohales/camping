#include "DialogReceiptEdit.h"
#include "ui_DialogReceiptEdit.h"
#include <QFile>

DialogReceiptEdit::DialogReceiptEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogReceiptEdit)
{
    ui->setupUi(this);
	
	QFile file(":/html/receipt.html");
	file.open(QFile::ReadOnly);
	
	ui->widget->setText(file.readAll());
}

DialogReceiptEdit::~DialogReceiptEdit()
{
    delete ui;
}
