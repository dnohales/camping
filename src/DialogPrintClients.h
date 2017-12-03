#ifndef DIALOGPRINTCLIENTS_H
#define DIALOGPRINTCLIENTS_H

#include "Client.h"
#include <QDialog>

namespace Ui
{
class DialogPrintClients;
}

class DialogPrintClients : public QDialog
{
	Q_OBJECT

public:
	explicit DialogPrintClients(const ClientCollection &list, QWidget *parent = 0);
	~DialogPrintClients();
	const ClientCollection *currentList() const;

public slots:
	void updateFilteredList();
	void updateInfo();
	void accept();

private:
	const ClientCollection originalList;
	ClientCollection filteredList;
	Ui::DialogPrintClients *ui;
};

#endif // DIALOGPRINTCLIENTS_H
