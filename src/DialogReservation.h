#ifndef DIALOGRESERVATION_H
#define DIALOGRESERVATION_H

#include "Reservation.h"
#include "Location.h"
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
	Reservation *reservation;
	Location::Type type;
	Ui::DialogReservation *ui;
};

#endif // DIALOGRESERVATION_H
