#ifndef DIALOGADDCLIENT_H
#define DIALOGADDCLIENT_H

#include <QDialog>
#include <QDate>
#include <QPushButton>
#include "Client.h"
#include "Location.h"
#include "Vehicle.h"

namespace Ui {
    class DialogClient;
}

class DialogClient : public QDialog
{
    Q_OBJECT

public:
    explicit DialogClient(Client *_client, Location::Type _type = Location::TENT, QWidget *parent = 0);
    ~DialogClient();
	
public slots:
	void updateOutDate(int days);
	void updateDaysCount();
	void searchLocation();
	void accept();
	void reset();
	
private slots:
	void onButtonBoxClicked(QAbstractButton*);
	void refreshWidgets();
	
private:
	Client *client;
	Location::Type type;
    Ui::DialogClient *ui;
};

#endif // DIALOGADDCLIENT_H
