#include "DialogReceiptEdit.h"
#include "main.h"
#include "ui_DialogReceiptEdit.h"
#include <QFile>
#include <QMessageBox>

DialogReceiptEdit::DialogReceiptEdit(QWidget *parent)
	: QDialog(parent),
	  ui(new Ui::DialogReceiptEdit)
{
	ui->setupUi(this);

	ui->buttonBox->button(QDialogButtonBox::Reset)->setText(tr("Restaurar al texto predeterminado"));
	ui->editor->setText(App()->config()->receiptTemplate());
}

DialogReceiptEdit::~DialogReceiptEdit()
{
	delete ui;
}

void DialogReceiptEdit::onButtonClicked(QAbstractButton *button)
{
	switch (ui->buttonBox->buttonRole(button)) {
	case QDialogButtonBox::ResetRole:
		if (QMessageBox::question(
				this,
				tr("Restaurar comprobante"),
				tr("¿Estás seguro que quieres restaurar el comprobante a sus valores de fábrica? perderás los cambios actuales."),
				QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
			App()->config()->restoreReceipt();
			App()->config()->save();
			ui->editor->setText(App()->config()->receiptTemplate());
		}
		break;
	case QDialogButtonBox::AcceptRole:
		App()->config()->setCustomReceipt(ui->editor->text());
		App()->config()->save();
		QDialog::accept();
		break;
	case QDialogButtonBox::RejectRole:
		QDialog::reject();
		break;
	default:
		break;
	}
}
