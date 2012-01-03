#ifndef CAMPINGAPPLICATION_H
#define CAMPINGAPPLICATION_H

#include <QApplication>

class CampingApplication : public QApplication
{
    Q_OBJECT
public:
	const static int DB_VERSION = 1;
	const static QString APP_NAME = "Administración Camping";
	
    explicit CampingApplication(QObject *parent = 0);
	bool initConfig();

signals:

public slots:
	
private:
	CampingConfig config;
};

#endif // CAMPINGAPPLICATION_H
