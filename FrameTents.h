#ifndef FRAMETENTS_H
#define FRAMETENTS_H

#include <QFrame>
#include "MainFrame.h"

namespace Ui {
    class FrameTents;
}

class FrameTents : public MainFrame
{
    Q_OBJECT

public:
    explicit FrameTents(QWidget *parent);
    ~FrameTents();

public slots:
	void onAddClicked();
	void refreshData();
	
private:
    Ui::FrameTents *ui;
};

#endif // FRAMETENTS_H
