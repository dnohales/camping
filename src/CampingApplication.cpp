#include <functional>

#include "CampingApplication.h"
#include "common.h"
#include "main.h"
#include <QPrintDialog>

CampingApplication::CampingApplication(int &argc, char **argv, int version)
	: QApplication(argc, argv, version)
{
	QSqlDatabase::addDatabase("QSQLITE");
	this->_initialized = false;

	this->setApplicationName("CampingAdmin");
	this->setOrganizationName("DamianNohales");
}

CampingConfig *CampingApplication::config()
{
	return &this->_config;
}

bool CampingApplication::isInitialized() const
{
	return this->_initialized;
}

void CampingApplication::setInitialized(bool ini)
{
	this->_initialized = ini;
}

void CampingApplication::initNewDatabase(QString filename)
{
	//Abrir archivo con la consulta
	QFile sqlFile(":/db/template.sql");
	if (!sqlFile.open(QFile::ReadOnly)) {
		throw CampingException(tr("No se puede obtener la plantilla de datos para crear la base de datos"));
	}

	//Abrir base de datos auxiliar "new"
	QFile databaseFile(filename);
	if (databaseFile.exists()) {
		databaseFile.remove();
	}
	QSqlDatabase newDatabase = QSqlDatabase::addDatabase("QSQLITE", "new");
	newDatabase.setDatabaseName(filename);
	if (!newDatabase.open()) {
		throw CampingException(tr("No se pudo crear el archivo para la base de datos"));
	}

	//Llenar base de datos nueva
	QString sQuery(sqlFile.readAll().data());
	try {
		this->execMulti(newDatabase, sQuery);
	} catch (CampingException &e) {
		throw CampingException(tr("No se pudo ejecutar la consulta que crea la base de datos: ") + e.message());
	}

	//Chequear base de datos
	try {
		this->checkDatabase(newDatabase);
		QSqlDatabase::cloneDatabase(QSqlDatabase::database("new"), "main");
		Db().open();
		this->config()->init();
		this->setInitialized(true);
	} catch (CampingException &e) {
		throw e;
	}
}

void CampingApplication::initExistentDatabase(QString filename)
{
	QFile databaseFile(filename);
	if (!databaseFile.exists()) {
		throw CampingException(tr("El archivo no existe."));
	}

	QSqlDatabase newDatabase = QSqlDatabase::addDatabase("QSQLITE", "new");
	newDatabase.setDatabaseName(filename);
	if (!newDatabase.open()) {
		throw CampingException(tr("No se pudo abrir la base de datos"));
	}

	try {
		this->checkDatabase(newDatabase);
		QSqlDatabase::cloneDatabase(QSqlDatabase::database("new"), "main");
		Db().open();
		this->config()->init();
		this->setInitialized(true);
	} catch (CampingException &e) {
		throw e;
	}
}

void CampingApplication::checkDatabase(QSqlDatabase &db)
{
	QSqlQuery dbVersionQuery(db);

	dbVersionQuery.prepare("SELECT value FROM config WHERE key = :key");
	dbVersionQuery.bindValue(":key", QVariant("db_version"));
	dbVersionQuery.exec();

	if (!dbVersionQuery.next()) {
		throw CampingException(tr("La base de datos no tiene número de versión"));
	}

	if (dbVersionQuery.value(0).toInt() != 2) {
		throw CampingException(tr("La base de datos es antigua"));
	}

	QStringList checkTables;
	checkTables << "client"
				<< "config"
				<< "location"
				<< "reservation"
				<< "vehicle";
	checkTables.sort();

	QStringList tables = db.tables();
	tables.sort();
	tables.removeOne("sqlite_sequence");

	if (checkTables != tables) {
		throw CampingException(tr("La base de datos está corrupta"));
	}
}

void CampingApplication::execMulti(QSqlDatabase &db, QString &query)
{
	QStringList lines = query.split("\n");
	QStringListIterator i(lines);
	QString strQuery("");
	QSqlQuery sqlQuery;

	db.transaction();
	while (i.hasNext()) {
		QString line = i.next();
		strQuery += line + "\n";
		if (line.endsWith(';')) {
			sqlQuery = db.exec(strQuery);
			if (sqlQuery.lastError().isValid()) {
				db.rollback();
				throw CampingException(sqlQuery.lastError().text());
			}
			strQuery = "";
		}
	}
	db.commit();
}

void CampingApplication::printHtml(QString html, QWidget *parent)
{
	_printer = new QPrinter();
	QScopedPointer<QPrintDialog> dialog(new QPrintDialog(_printer, parent));
	if (dialog->exec() != QDialog::Accepted) {
		delete _printer;
		return;
	}
	_printPage = new QWebEnginePage(parent);
	connect(_printPage, SIGNAL(loadFinished(bool)), SLOT(onPrintPageLoaded(bool)));
	_printPage->setHtml(html);
}

void CampingApplication::onPrintPageLoaded(bool ok)
{
	if (ok) {
		_printPage->print(_printer, [this](bool) {
			delete this->_printer;
			delete this->_printPage;
		});
	}
}
