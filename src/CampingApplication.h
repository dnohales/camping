#ifndef CAMPINGAPPLICATION_H
#define CAMPINGAPPLICATION_H

#include "CampingConfig.h"
#include <QApplication>
#include <QPrinter>
#include <QSqlDatabase>
#include <QWebEnginePage>

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

private slots:
	void onPrintPageLoaded(bool ok);

private:
	QPrinter *_printer;
	QWebEnginePage *_printPage;
	CampingConfig _config;
	bool _initialized;
};

#endif // CAMPINGAPPLICATION_H
