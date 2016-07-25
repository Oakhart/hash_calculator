#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fileDialog = new QFileDialog(this);
    threadTimer = new QTimer(this);

    threadTimer->setSingleShot(false);

    connect( fileDialog, SIGNAL(currentChanged(QString)), this, SLOT(onFileSelected(QString)) );
    connect( threadTimer, SIGNAL(timeout()), this, SLOT(onTick()));

    calculating = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toolButton_clicked()
{
    fileDialog->show();
}

void MainWindow::onFileSelected(const QString & path)
{
    this->fileName = path;
    ui->lineEdit->setText(fileName);

}

void MainWindow::calculate_MD5_and_SHA1()
{
    for(int i = 0; i < 2; ++i){


        hashCalculatorVector.push_back( new HashCalculator(this) );

        threadVector.push_back( new QThread(this) );

        hashCalculatorVector.back()->setFileName(fileName);

        if(i == 0){
            hashCalculatorVector.back()->setHashAlgorithm(QCryptographicHash::Md5);
        } else {
            hashCalculatorVector.back()->setHashAlgorithm(QCryptographicHash::Sha1);
        }

        hashCalculatorVector.back()->init(threadVector.back());

        threadVector.back()->start();

    }
    threadTimer->start(500);
    resultDialog = new ResultDialog(this);
    resultDialog->setAlgorithms(QCryptographicHash::Md5, QCryptographicHash::Sha1);
}

void MainWindow::on_pushButton_clicked()
{
    if(!calculating){

        calculating = true;
        hashCalculatorVector.clear();
        threadVector.clear();
        if(fileName.size() == 0){
            QMessageBox::warning(this, "File Not Selected", "There is no file selected");
            return;
        }
        QFileInfo check(fileName);
        if(!check.exists() || !check.isFile()){
            QMessageBox::warning(this, "File does not exist!", fileName + " does not exist");
            return;
        }

        if(ui->radioButton_Both->isChecked()){

            calculate_MD5_and_SHA1();
        }
        pleaseWaitDialog = new PleaseWaitDialog(this);
        pleaseWaitDialog->show();
    }
}

void  MainWindow::onTick()
{
    bool isHashCalculated = true;
    foreach(QThread* thr, threadVector){
        if(thr->isRunning()){
            isHashCalculated = false;
            return;
        }
    }

    QString hashStrings[2];

    int iterator = 0;

    foreach(HashCalculator *hash, hashCalculatorVector){
        hashStrings[iterator].append( hash->hashValue.toHex() );
        iterator++;
    }

    if(isHashCalculated){
        pleaseWaitDialog->close();
        resultDialog->setChecksums(hashStrings[0], hashStrings[1]);
        resultDialog->show();
        threadTimer->stop();


    }
}
