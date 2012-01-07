#ifndef FRAMEDORMS_H
#define FRAMEDORMS_H

#include <QFrame>
#include "MainFrame.h"

namespace Ui {
    class FrameDorms;
}

class FrameDorms : public MainFrame
{
    Q_OBJECT

public:
    explicit FrameDorms(QWidget *parent = 0);
    ~FrameDorms();
	
public slots:
	void refreshData();

private slots:
	void on_buttonAdd_clicked();
	
private:
    Ui::FrameDorms *ui;
};

#endif // FRAMEDORMS_H
