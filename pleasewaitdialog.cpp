#include "pleasewaitdialog.h"
#include "ui_pleasewaitdialog.h"

PleaseWaitDialog::PleaseWaitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PleaseWaitDialog)
{
    ui->setupUi(this);

    // sets up and connects the timer
    timer = new QTimer(this);
    timer->setSingleShot(false);

    connect(timer, SIGNAL(timeout()), this, SLOT(onTick()) );

    //sets up the progress bar
    ui->progressBar->setRange(0,100);

}

PleaseWaitDialog::~PleaseWaitDialog()
{
    delete ui;
}

void PleaseWaitDialog::onTick()
{
    // increases or decreases the progress bar value, depending on the progress bar direction
    if(ascending){
        progressBarValue++;
    } else {
        progressBarValue--;
    }

    // when the progress bar value reaches maximum, changes the progress bar appearance, and the value starts to decrease
    if(progressBarValue == 100){
        ascending = false;
        ui->progressBar->setInvertedAppearance(true);
    }

    // when the value reaches 0, changes the progress bar appearance, and the value starts to increase
    if(progressBarValue == 0){
        ascending = true;
        ui->progressBar->setInvertedAppearance(false);
    }

    // sets the progress bar new value
    ui->progressBar->setValue(progressBarValue);
}

void PleaseWaitDialog::on_pushButton_clicked()
{
    // closes the dialog window. When the window is closed, there is no neeed for the timer to run
    emit closePleasWaitDialog();
    this->close();
    timer->stop();
}

void PleaseWaitDialog::reinit()
{
    //resets the progress bar
    progressBarValue = 1;
    ui->progressBar->setInvertedAppearance(false);
    ui->progressBar->setValue(progressBarValue);
    ascending = true;

    //starts the timer
    timer->start(8);

    //shows the dialog window
    this->show();
}
