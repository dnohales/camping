#ifndef DIALOGPRINTCLIENTS_H
#define DIALOGPRINTCLIENTS_H

#include <QDialog>

namespace Ui {
    class DialogPrintClients;
}

class DialogPrintClients : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPrintClients(QWidget *parent = 0);
    ~DialogPrintClients();

private:
    Ui::DialogPrintClients *ui;
};

#endif // DIALOGPRINTCLIENTS_H
