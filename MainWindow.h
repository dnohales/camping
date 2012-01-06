#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "FrameTents.h"
#include "FrameDorms.h"

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
	void onAbout();
	void refreshInitializedState();
	void showTents();
	void showDorms();
	
signals:
	void textSearched(QString query);
	void fileOpened(QString filename);

private slots:
	void on_lineEditSearch_textChanged(QString );
	void onFileOpened(QString filename);
	
private:
	void openExistentFile(QString filename);
	
private:
	QTimer *_searchTimer;
	FrameTents *frameTents;
	FrameDorms *frameDorms;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
