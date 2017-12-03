#ifndef DIALOGADDCLIENT_H
#define DIALOGADDCLIENT_H

#include "Client.h"
#include "Location.h"
#include "Vehicle.h"
#include <QDate>
#include <QDialog>
#include <QPushButton>
#include <QStandardItemModel>

namespace Ui
{
class DialogClient;
}

class DialogClient : public QDialog
{
	Q_OBJECT

public:
	explicit DialogClient(Client *_client, Location::Type _type = Location::TENT, QWidget *parent = 0);
	~DialogClient();

	bool isVehicleRowEmpty(int row);

public slots:
	void updateOutDate(int days);
	void updateDaysCount();
	void searchLocation();
	void accept();
	void reset();

private slots:
	void onButtonBoxClicked(QAbstractButton *);
	void refreshWidgets();
	void on_buttonVehicleAdd_clicked();
	void on_buttonVehicleDelete_clicked();
	void on_editLocation_textChanged(QString);

private:
	Client *client;
	Location::Type type;
	Ui::DialogClient *ui;
};

#endif // DIALOGADDCLIENT_H
