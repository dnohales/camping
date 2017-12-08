#ifndef DIALOGCLIENT_H
#define DIALOGCLIENT_H

#include "Client.h"
#include <QAbstractButton>
#include <QDialog>

namespace Ui
{
class DialogClient;
}

class DialogClient : public QDialog
{
	Q_OBJECT

public:
	explicit DialogClient(Client *_client, QWidget *parent = 0);
	~DialogClient();

public slots:
	void onButtonBoxClicked(QAbstractButton *);
	void accept();
	void reset();

private slots:
	void refreshWidgets();

private:
	Client *client;
	Ui::DialogClient *ui;
};

#endif // DIALOGCLIENT_H
