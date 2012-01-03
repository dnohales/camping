#ifndef CAMPINGAPPLICATION_H
#define CAMPINGAPPLICATION_H

#include <QApplication>

class CampingConfig;

class CampingApplication : public QApplication
{
    Q_OBJECT
public:
	int dbVersion() const { return 1; }
	QString name() const { return "Administración Camping"; }
	
    explicit CampingApplication(QObject *parent = 0);
	CampingConfig *config() const;
	void initNewDatabase();
	void initExistentDatabase(QString filename);

signals:

public slots:
	
private:
	CampingConfig *_config;
};

#endif // CAMPINGAPPLICATION_H
