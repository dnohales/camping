#ifndef FRAMERESERVATIONS_H
#define FRAMERESERVATIONS_H

#include "MainFrame.h"

class FrameReservations : public MainFrame
{
	Q_OBJECT
public:
	explicit FrameReservations(QWidget *parent = 0);

	SqlCriteria baseCriteria(Location::Type findType = Location::ALL);
	void doCreateReservation(Reservation &c, Location::Type type);
	void doEditReservation(Reservation &c);
	void doPrintReceipt(Reservation &c);
	void doDeleteReservation(Reservation &c);
};

#endif // FRAMERESERVATIONS_H
