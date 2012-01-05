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
	
private:
	QString getCampingSaveFileName();
	
public slots:
	void onSearchTimeout();
	void onOpenFile();
	void onNewFile();
	void onFileSaveAs();
	void refreshInitializedState();
	
signals:
	void textSearched(QString query);

private slots:
	void on_lineEditSearch_textChanged(QString );
	
private:
	QTimer *_searchTimer;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
