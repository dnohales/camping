#ifndef DIALOGRESERVATION_H
#define DIALOGRESERVATION_H

#include "Location.h"
#include "Reservation.h"
#include "Vehicle.h"
#include <QDate>
#include <QDialog>
#include <QPushButton>
#include <QStandardItemModel>

namespace Ui
{
class DialogReservation;
}

class DialogReservation : public QDialog
{
	Q_OBJECT

public:
	explicit DialogReservation(Reservation *_reservation, Location::Type _type = Location::TENT, QWidget *parent = 0);
	~DialogReservation();

	bool isVehicleRowEmpty(int row);

public slots:
	void updateOutDate(int days);
	void updateDaysCount();
	void accept();
	void reset();

private slots:
	void onClientCompleterActivated(const QModelIndex &index);
	void onButtonBoxClicked(QAbstractButton *);
	void refreshClientWidgets();
	void refreshWidgets();
	void on_buttonVehicleAdd_clicked();
	void on_buttonVehicleDelete_clicked();
	void on_editLocation_textChanged(QString);
	void on_buttonCompletedClientReset_clicked();

private:
	Client completedClient;
	Reservation *reservation;
	Location::Type type;
	Ui::DialogReservation *ui;
};

#endif // DIALOGRESERVATION_H
