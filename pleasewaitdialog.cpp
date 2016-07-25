#include "pleasewaitdialog.h"
#include "ui_pleasewaitdialog.h"

PleaseWaitDialog::PleaseWaitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PleaseWaitDialog)
{
    ui->setupUi(this);

    progressBarValue = 1;
    ascending = true;

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
{
    ui->progressBar->setInvertedAppearance(false);
    ui->progressBar->setValue(0);
    timer->start(16);
}
