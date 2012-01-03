#ifndef CAMPINGCONFIG_H
#define CAMPINGCONFIG_H

#include <QObject>
#include <QMap>

class CampingConfig : public QObject
{
    Q_OBJECT
public:
    explicit CampingConfig(QObject *parent = 0);
	void init();
	void clear();
	void save();
	int dbVersion();
	
signals:

public slots:
	void setDbVersion(int version);
	
private:
	QMap<QString, QVariant> data;
};

#endif // CAMPINGCONFIG_H
