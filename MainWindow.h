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

signals:
	void textSearched(QString query);
	void fileOpened(QString filename);
	
public slots:
	void refreshInitializedState();
	void showTents();
	void showDorms();
	void requestRefresh();
	
private slots:
	void onSearchTimeout();
	void onOpenFile();
	void onNewFile();
	void onFileSaveAs();
	void on_lineEditSearch_textChanged(QString );
	void onFileOpened(QString filename);
	void on_actionAcerca_de_triggered();
	void on_actionAcerca_de_Qt_triggered();
	void on_actionPrintClients_triggered();
	
	void on_actionReceiptEdit_triggered();
	
private:
	void openExistentFile(QString filename);
	
private:
	QTimer *_searchTimer;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
