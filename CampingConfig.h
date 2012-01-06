#ifndef CAMPINGCONFIG_H
#define CAMPINGCONFIG_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QVariant>

class CampingConfig : public QObject
{
    Q_OBJECT
public:
    explicit CampingConfig(QObject *parent = 0);
	void init();
	void clear();
	void save();
	int dbVersion();
	QString lastFilename();
	
signals:

public slots:
	void setDbVersion(int version);
	void setLastFilename(QString filename);
	
private:
	QMap<QString, QVariant> data;
};

#endif // CAMPINGCONFIG_H
