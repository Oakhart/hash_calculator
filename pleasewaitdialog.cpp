#include "pleasewaitdialog.h"
#include "ui_pleasewaitdialog.h"

PleaseWaitDialog::PleaseWaitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PleaseWaitDialog)
{
    ui->setupUi(this);
}

PleaseWaitDialog::~PleaseWaitDialog()
{
    delete ui;
}
