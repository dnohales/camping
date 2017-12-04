#ifndef DIALOGPRINTRESERVATIONS_H
#define DIALOGPRINTRESERVATIONS_H

#include "Reservation.h"
#include <QDialog>

namespace Ui
{
class DialogPrintReservations;
}

class DialogPrintReservations : public QDialog
{
	Q_OBJECT

public:
	explicit DialogPrintReservations(const ReservationCollection &list, QWidget *parent = 0);
	~DialogPrintReservations();
	const ReservationCollection *currentList() const;

public slots:
	void updateFilteredList();
	void updateInfo();
	void accept();

private:
	const ReservationCollection originalList;
	ReservationCollection filteredList;
	Ui::DialogPrintReservations *ui;
};

#endif // DIALOGPRINTRESERVATIONS_H
