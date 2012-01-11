#ifndef DIALOGRECEIPTEDIT_H
#define DIALOGRECEIPTEDIT_H

#include <QDialog>

namespace Ui {
    class DialogReceiptEdit;
}

class DialogReceiptEdit : public QDialog
{
    Q_OBJECT

public:
    explicit DialogReceiptEdit(QWidget *parent = 0);
    ~DialogReceiptEdit();

private:
    Ui::DialogReceiptEdit *ui;
};

#endif // DIALOGRECEIPTEDIT_H
