#ifndef CAMPINGCONFIG_H
#define CAMPINGCONFIG_H

#include <QMap>
#include <QObject>
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
	QString receiptTemplate();

signals:

public slots:
	void setDbVersion(int version);
	void setLastFilename(QString filename);
	void setCustomReceipt(QString receipt);
	void restoreReceipt();

private:
	QMap<QString, QVariant> data;
};

#endif // CAMPINGCONFIG_H
