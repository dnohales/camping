#ifndef DIALOGCLIENTSELECTOR_H
#define DIALOGCLIENTSELECTOR_H

#include <QDialog>
#include <QListWidget>

namespace Ui {
	class DialogClientSelector;
}

class DialogClientSelector : public QDialog
{
	Q_OBJECT

public:
	explicit DialogClientSelector(QString idList, QWidget *parent = 0);
	~DialogClientSelector();
	int selectedId();

public slots:
	int exec();
	void accept();

private slots:
	void on_list_itemActivated(QListWidgetItem* item);

private:
	int _selectedId;
	Ui::DialogClientSelector *ui;
};

#endif // DIALOGCLIENTSELECTOR_H
