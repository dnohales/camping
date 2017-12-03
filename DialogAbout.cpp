#include "DialogAbout.h"
#include "ui_DialogAbout.h"

DialogAbout::DialogAbout(QWidget *parent)
	: QDialog(parent),
	  ui(new Ui::DialogAbout)
{
	ui->setupUi(this);
	ui->buttonBox->button(QDialogButtonBox::Close)->setText(tr("Cerrar"));
}

DialogAbout::~DialogAbout()
{
	delete ui;
}

void DialogAbout::on_buttonBox_clicked(QAbstractButton * /*button*/)
{
	this->accept();
}
