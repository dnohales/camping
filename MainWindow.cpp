#include "main.h"
#include "common.h"
#include "MainWindow.h"
#include "DialogClient.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	this->setWindowTitle(App()->name());
	
	this->_searchTimer = new QTimer();
	this->_searchTimer->setSingleShot(true);
	this->_searchTimer->setInterval(500);
	
	this->connect(this->_searchTimer, SIGNAL(timeout()), SLOT(onSearchTimeout()));
	this->connect(App(), SIGNAL(initializedChanged()), SLOT(refreshInitializedState()));
	this->connect(this, SIGNAL(fileOpened(QString)), SLOT(onFileOpened(QString)));
	
	if(!App()->config()->lastFilename().isEmpty()){
		try{
			App()->initExistentDatabase(App()->config()->lastFilename());
			emit fileOpened(App()->config()->lastFilename());
		} catch(...){}
	}
	
	this->refreshInitializedState();
}

MainWindow::~MainWindow()
{
	delete this->_searchTimer;
    delete ui;
}

void MainWindow::on_lineEditSearch_textChanged(QString )
{
	this->_searchTimer->stop();
    this->_searchTimer->start();
}

void MainWindow::onSearchTimeout()
{
	emit textSearched(this->ui->lineEditSearch->text());
}

void MainWindow::onOpenFile()
{
	QString filename = QFileDialog::getOpenFileName(
		this,
		tr("Abrir un archivo de camping"),
		QString(),
		tr("Archivo de camping (*.%1);;Todos los archivos (*)").arg(App()->fileExtension())
	);
	if(!filename.isEmpty()){
		try{
			App()->initExistentDatabase(filename);
			emit fileOpened(filename);
		} catch(CampingException &e){
			QMessageBox::critical(this, App()->name(), tr("No se puede abrir \"%1\": %2.").arg(filename, e.message()) );
		}
	}
}

void MainWindow::onFileSaveAs()
{
	QString filename = this->getCampingSaveFileName();
	if(!filename.isNull()){
		try{
			QFile::copy(Db().databaseName(), filename);
			App()->initExistentDatabase(filename);
			emit fileOpened(filename);
		} catch(CampingException &e){
			QMessageBox::critical(this, App()->name(), tr("No se pudo recargar la base de datos \"%1\": %2.").arg(filename, e.message()) );
		}
	}
}

void MainWindow::onNewFile()
{
	QString filename = this->getCampingSaveFileName();
	if(!filename.isEmpty()){
		try{
			App()->initNewDatabase(filename);
			emit fileOpened(filename);
		} catch(CampingException &e){
			QMessageBox::critical(this, App()->name(), tr("No se puede crear la base de datos en \"%1\": %2.").arg(filename, e.message()) );
		}
	}
}

QString MainWindow::getCampingSaveFileName()
{
	QString filename = QFileDialog::getSaveFileName(
		this,
		tr("Seleccione la ubicación donde guardar el camping"),
		QString(),
		tr("Archivo de camping (*.%1);;Todos los archivos (*)").arg(App()->fileExtension())
	);
	
	if( !filename.isNull() && !filename.endsWith(App()->fileExtensionWithDot()) ){
		filename += App()->fileExtensionWithDot();
	}
	
	if( QFile::exists(filename) && QMessageBox::question(
			this,
			App()->name(),
			tr("Ya existe el archivo \"%1\" ¿Desea reemplazarlo?").arg(QFileInfo(filename).completeBaseName()),
	        QMessageBox::Yes, QMessageBox::No
	) == QMessageBox::No){
		filename = "";
	}
	
	return filename;
}

void MainWindow::refreshInitializedState()
{
	bool ini = App()->isInitialized();
	
	this->ui->menuCamping->setEnabled(ini);
	this->ui->menuReportes->setEnabled(ini);
	this->ui->actionGuardar_camping_como->setEnabled(ini);
	this->ui->frameInitialized->setVisible(ini);
	this->ui->frameNotInitialized->setHidden(ini);
}

void MainWindow::onAbout()
{
	Client c(false);
	DialogClient dialog(&c);
	
	dialog.exec();
}

void MainWindow::onFileOpened(QString filename)
{
	App()->config()->setLastFilename(filename);
	this->setWindowTitle(App()->name() + " [" + QFileInfo(filename).fileName() + "]");
}
