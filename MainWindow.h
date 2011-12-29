#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
	
public slots:
	void onSearch(QString query);
	void onSearchTimeout();
	
signals:
	void textSearched(QString query);

private slots:
	void on_lineEditSearch_textChanged(QString );
	
private:
	QTimer *m_search_timer;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
