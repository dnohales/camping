#include <QMessageBox>
#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	
	this->m_search_timer = new QTimer();
	this->m_search_timer->setSingleShot(true);
	this->m_search_timer->setInterval(500);
	
	this->connect(this->m_search_timer, SIGNAL(timeout()), SLOT(onSearchTimeout()));
	this->connect(this, SIGNAL(textSearched(QString)), SLOT(onSearch(QString)));
}

MainWindow::~MainWindow()
{
	delete this->m_search_timer;
    delete ui;
}

void MainWindow::on_lineEditSearch_textChanged(QString )
{
	this->m_search_timer->stop();
    this->m_search_timer->start();
}

void MainWindow::onSearchTimeout()
{
	emit textSearched(this->ui->lineEditSearch->text());
}
