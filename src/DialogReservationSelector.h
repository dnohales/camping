#ifndef DIALOGRESERVATIONSELECTOR_H
#define DIALOGRESERVATIONSELECTOR_H

#include <QDialog>
#include <QListWidget>

namespace Ui
{
class DialogReservationSelector;
}

class DialogReservationSelector : public QDialog
{
	Q_OBJECT

public:
	explicit DialogReservationSelector(QString idList, QWidget *parent = 0);
	~DialogReservationSelector();
	int selectedId();

public slots:
	int exec();
	void accept();

private slots:
	void on_list_itemActivated(QListWidgetItem *item);

private:
	int _selectedId;
	Ui::DialogReservationSelector *ui;
};

#endif // DIALOGRESERVATIONSELECTOR_H
