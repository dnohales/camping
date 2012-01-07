#include "FrameDorms.h"
#include "ui_FrameDorms.h"

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
