#ifndef FRAMETENTS_H
#define FRAMETENTS_H

#include <QFrame>

namespace Ui {
    class FrameTents;
}

class FrameTents : public QFrame
{
    Q_OBJECT

public:
    explicit FrameTents(QWidget *parent = 0);
    ~FrameTents();

private:
    Ui::FrameTents *ui;
};

#endif // FRAMETENTS_H
