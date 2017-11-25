#ifndef DIALOGRECEIPTEDIT_H
#define DIALOGRECEIPTEDIT_H

#include <QDialog>
#include <QPushButton>

namespace Ui {
	class DialogReceiptEdit;
}

class DialogReceiptEdit : public QDialog
{
	Q_OBJECT

public:
	explicit DialogReceiptEdit(QWidget *parent = 0);
	~DialogReceiptEdit();

public slots:
	void onButtonClicked(QAbstractButton *button);

private:
	Ui::DialogReceiptEdit *ui;
};

#endif // DIALOGRECEIPTEDIT_H
