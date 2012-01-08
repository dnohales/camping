#ifndef CAMPINGAPPLICATION_H
#define CAMPINGAPPLICATION_H

#include <QApplication>
#include <QSqlDatabase>
#include <QPrinter>
#include "CampingConfig.h"

class CampingConfig;

class CampingApplication : public QApplication
{
    Q_OBJECT
public:
	int dbVersion() const { return 1; }
	QString name() const { return tr("Administración de Camping"); }
	QString fileExtension() const { return "camp"; }
	QString fileExtensionWithDot() const { return QString(".") + this->fileExtension(); }
	
    CampingApplication(int &argc, char **argv, int = QT_VERSION);
	CampingConfig *config();
	QPrinter *printer();
	void printHtml(QString html, QWidget *parent = 0);
	void initNewDatabase(QString filename);
	void initExistentDatabase(QString filename);
	void checkDatabase(QSqlDatabase &db);
	bool isInitialized() const;
	void setInitialized(bool ini);
private:
	void execMulti(QSqlDatabase &db, QString &query);
	
signals:
	void initializedChanged();

public slots:
	
private:
	QPrinter _printer;
	CampingConfig _config;
	bool _initialized;
};

#endif // CAMPINGAPPLICATION_H
