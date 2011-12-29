#include "FrameDorms.h"
#include "ui_FrameDorms.h"

FrameDorms::FrameDorms(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameDorms)
{
    ui->setupUi(this);
}

FrameDorms::~FrameDorms()
{
    delete ui;
}
