#include "FrameTents.h"
#include "ui_FrameTents.h"

FrameTents::FrameTents(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameTents)
{
    ui->setupUi(this);
}

FrameTents::~FrameTents()
{
    delete ui;
}
