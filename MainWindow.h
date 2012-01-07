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
	
	QString searchQuery();
	
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
	void refreshData();
	void requestRefresh();
	
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
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
