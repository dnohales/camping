#include "MainWindow.h"
#include "DialogAbout.h"
#include "DialogClient.h"
#include "DialogPrintClients.h"
#include "DialogReceiptEdit.h"
#include "common.h"
#include "main.h"
#include "ui_MainWindow.h"
#include <QPageSetupDialog>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	  ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	this->setWindowTitle(App()->name());

	this->_searchTimer = new QTimer();
	this->_searchTimer->setSingleShot(true);
	this->_searchTimer->setInterval(300);

	this->ui->frameTents->setMainParent(this);
	this->ui->frameDorms->setMainParent(this);

	this->connect(this->_searchTimer, SIGNAL(timeout()), SLOT(onSearchTimeout()));
	this->connect(this, SIGNAL(fileOpened(QString)), SLOT(onFileOpened(QString)));
	this->connect(ui->frameTents, SIGNAL(refreshed()), SLOT(requestRefresh()));
	this->connect(ui->frameDorms, SIGNAL(refreshed()), SLOT(requestRefresh()));
	this->connect(this, SIGNAL(textSearched(QString)), SLOT(requestRefresh()));

	if (!App()->config()->lastFilename().isEmpty()) {
		try {
			App()->initExistentDatabase(App()->config()->lastFilename());
			emit fileOpened(App()->config()->lastFilename());
		} catch (...) {
		}
	}

	QSettings settings;
	if (settings.value("Window/Maximized").toBool()) {
		this->showMaximized();
	} else {
		this->resize(settings.value("Window/Width").toInt(), settings.value("Window/Height").toInt());
	}

	this->refreshInitializedState();
}

MainWindow::~MainWindow()
{
	QSettings settings;
	settings.setValue("Window/Width", this->width());
	settings.setValue("Window/Height", this->height());
	settings.setValue("Window/Maximized", this->isMaximized());

	delete this->_searchTimer;
	delete ui;
}

void MainWindow::on_lineEditSearch_textChanged(QString)
{
	this->_searchTimer->stop();
	this->_searchTimer->start();
}

void MainWindow::onSearchTimeout()
{
	emit textSearched(this->ui->lineEditSearch->text());
}

QString MainWindow::searchQuery()
{
	return this->ui->lineEditSearch->text().simplified();
}

void MainWindow::onOpenFile()
{
	QString filename = QFileDialog::getOpenFileName(
		this,
		tr("Abrir un archivo de camping"),
		QString(),
		tr("Archivo de camping (*.%1);;Todos los archivos (*)").arg(App()->fileExtension()));
	if (!filename.isEmpty()) {
		try {
			App()->initExistentDatabase(filename);
			emit fileOpened(filename);
		} catch (CampingException &e) {
			QMessageBox::critical(this, App()->name(), tr("No se puede abrir \"%1\": %2.").arg(filename, e.message()));
		}
	}
}

void MainWindow::onFileSaveAs()
{
	QString filename = this->getCampingSaveFileName();
	if (!filename.isNull()) {
		try {
			QFile::copy(Db().databaseName(), filename);
			App()->initExistentDatabase(filename);
			emit fileOpened(filename);
		} catch (CampingException &e) {
			QMessageBox::critical(this, App()->name(), tr("No se pudo recargar la base de datos \"%1\": %2.").arg(filename, e.message()));
		}
	}
}

void MainWindow::onNewFile()
{
	QString filename = this->getCampingSaveFileName();
	if (!filename.isEmpty()) {
		try {
			App()->initNewDatabase(filename);
			emit fileOpened(filename);
		} catch (CampingException &e) {
			QMessageBox::critical(this, App()->name(), tr("No se puede crear la base de datos en \"%1\": %2.").arg(filename, e.message()));
		}
	}
}

QString MainWindow::getCampingSaveFileName()
{
	QString filename = QFileDialog::getSaveFileName(
		this,
		tr("Seleccione la ubicación donde guardar el camping"),
		QString(),
		tr("Archivo de camping (*.%1);;Todos los archivos (*)").arg(App()->fileExtension()));

	if (!filename.isNull() && !filename.endsWith(App()->fileExtensionWithDot())) {
		filename += App()->fileExtensionWithDot();
	}

	if (QFile::exists(filename) && QMessageBox::question(
									   this,
									   App()->name(),
									   tr("Ya existe el archivo \"%1\" ¿Desea reemplazarlo?").arg(QFileInfo(filename).completeBaseName()),
									   QMessageBox::Yes, QMessageBox::No) == QMessageBox::No) {
		filename = "";
	}

	return filename;
}

void MainWindow::refreshInitializedState()
{
	bool ini = App()->isInitialized();

	this->ui->menuCamping->setEnabled(ini);
	this->ui->actionGuardar_camping_como->setEnabled(ini);
	this->ui->frameInitialized->setVisible(ini);
	this->ui->frameNotInitialized->setHidden(ini);
	this->ui->frameTents->setVisible(ini);
	this->ui->frameDorms->setVisible(ini);

	if (ini) {
		this->showTents();
	}
}

void MainWindow::onFileOpened(QString filename)
{
	App()->config()->setLastFilename(filename);
	this->setWindowTitle(App()->name() + " [" + QFileInfo(filename).fileName() + "]");
	this->requestRefresh();
}

void MainWindow::showTents()
{
	this->ui->frameTents->setVisible(true);
	this->ui->pushButtonTents->setChecked(true);

	this->ui->frameDorms->setVisible(false);
	this->ui->pushButtonDorms->setChecked(false);

	this->ui->actionPrintClients->setEnabled(true);

	if (!this->ui->frameTents->isRefreshed()) {
		this->ui->frameTents->refreshData();
	}
}

void MainWindow::showDorms()
{
	this->ui->frameTents->setVisible(false);
	this->ui->pushButtonTents->setChecked(false);

	this->ui->frameDorms->setVisible(true);
	this->ui->pushButtonDorms->setChecked(true);

	this->ui->actionPrintClients->setEnabled(false);

	if (!this->ui->frameDorms->isRefreshed()) {
		this->ui->frameDorms->refreshData();
	}
}

void MainWindow::requestRefresh()
{
	this->ui->frameTents->requestRefresh();
	this->ui->frameDorms->requestRefresh();

	if (this->ui->frameTents->isVisible() && !this->ui->frameTents->isRefreshed()) {
		this->ui->frameTents->refreshData();
	} else if (this->ui->frameDorms->isVisible() && !this->ui->frameDorms->isRefreshed()) {
		this->ui->frameDorms->refreshData();
	}
}

void MainWindow::on_actionAcerca_de_triggered()
{
	DialogAbout dialog(this);
	dialog.exec();
}

void MainWindow::on_actionAcerca_de_Qt_triggered()
{
	CampingApplication::aboutQt();
}

void MainWindow::on_actionPrintClients_triggered()
{
	DialogPrintClients dialog(ui->frameTents->currentList(), this);
	dialog.exec();
}

void MainWindow::on_actionReceiptEdit_triggered()
{
	DialogReceiptEdit dialog(this);
	dialog.exec();
}

void MainWindow::on_actionPedir_soporte_triggered()
{
	QMessageBox::information(this, "Soporte", "Para pedir soporte puede enviar un e-mail a damiannohales@gmail.com.");
}
