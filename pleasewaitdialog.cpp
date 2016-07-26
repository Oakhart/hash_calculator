#include "pleasewaitdialog.h"
#include "ui_pleasewaitdialog.h"

PleaseWaitDialog::PleaseWaitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PleaseWaitDialog)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    timer->setSingleShot(false);

    connect(timer, SIGNAL(timeout()), this, SLOT(onTick()) );
    ui->progressBar->setRange(0,100);

}

PleaseWaitDialog::~PleaseWaitDialog()
{
    delete ui;
}

void PleaseWaitDialog::onTick()
{
    if(ascending){
        progressBarValue++;
    } else {
        progressBarValue--;
    }

    if(progressBarValue == 100){
        ascending = false;
        ui->progressBar->setInvertedAppearance(true);
    }

    if(progressBarValue == 0){
        ascending = true;
        ui->progressBar->setInvertedAppearance(false);
    }

    ui->progressBar->setValue(progressBarValue);
}

void PleaseWaitDialog::on_pushButton_clicked()
{
    emit closePleasWaitDialog();
    timer->stop();
}

void PleaseWaitDialog::reinit()
{   progressBarValue = 1;
    ui->progressBar->setInvertedAppearance(false);
    ui->progressBar->setValue(progressBarValue);
    ascending = true;
    timer->start(8);
    this->show();

}
