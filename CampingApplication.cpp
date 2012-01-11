#include "main.h"
#include "common.h"
#include "CampingApplication.h"
#include <QPrintDialog>
#include <QtWebKit/QWebView>

CampingApplication::CampingApplication(int &argc, char **argv, int version) : 
    QApplication(argc, argv, version)
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

QPrinter * CampingApplication::printer()
{
	return &this->_printer;
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
	//Abrir archiv con la consulta
	QFile sqlFile(":/db/template.sql");
	if( !sqlFile.open(QFile::ReadOnly) ){
		throw CampingException(tr("No se puede obtener la plantilla de datos para crear la base de datos"));
	}
	
	//Abrir base de datos auxiliar "new"
	QFile databaseFile(filename);
	if(databaseFile.exists()){
		databaseFile.remove();
	}
	QSqlDatabase newDatabase = QSqlDatabase::addDatabase("QSQLITE", "new");
	newDatabase.setDatabaseName(filename);
	if(!newDatabase.open()){
		throw CampingException(tr("No se pudo crear el archivo para la base de datos"));
	}
	
	//Llenar base de datos nueva
	QString sQuery(sqlFile.readAll().data());
	try{
		this->execMulti(newDatabase, sQuery);
	} catch(CampingException &e){
		throw CampingException(tr("No se pudo ejecutar la consulta que crea la base de datos: ")+e.message());
	}

	//Chequear base de datos
	try{
		this->checkDatabase(newDatabase);
		QSqlDatabase::cloneDatabase(QSqlDatabase::database("new"), "main");
		Db().open();
		this->config()->init();
		this->setInitialized(true);
	} catch(CampingException &e){
		throw e;
	}
}

void CampingApplication::initExistentDatabase(QString filename)
{
	QFile databaseFile(filename);
	if(!databaseFile.exists()){
		throw CampingException(tr("El archivo no existe."));
	}
	
	QSqlDatabase newDatabase = QSqlDatabase::addDatabase("QSQLITE", "new");
	newDatabase.setDatabaseName(filename);
	if(!newDatabase.open()){
		throw CampingException(tr("No se pudo abrir la base de datos"));
	}
	
	try{
		this->checkDatabase(newDatabase);
		QSqlDatabase::cloneDatabase(QSqlDatabase::database("new"), "main");
		Db().open();
		this->config()->init();
		this->setInitialized(true);
	} catch(CampingException &e){
		throw e;
	}
}

void CampingApplication::checkDatabase(QSqlDatabase &db)
{
	QStringList checkTables;
	checkTables << "client" << "config" << "location" << "vehicle";
	checkTables.sort();
	
	QStringList tables = db.tables();
	tables.sort();
	tables.removeOne("sqlite_sequence");
	
	if(checkTables != tables){
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
	while(i.hasNext()){
		QString line = i.next();
		strQuery += line + "\n";
		if(line.endsWith(';')){
			sqlQuery = db.exec(strQuery);
			if(sqlQuery.lastError().isValid()){
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
	QPrintDialog dialog(this->printer(), parent);
	dialog.exec();
	
	if(dialog.result() == QPrintDialog::Accepted){
		QWebView webview(parent);
		webview.setHtml(html);
		
		webview.print(this->printer());
	}
}

