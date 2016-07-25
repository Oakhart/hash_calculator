#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pleaseWaitDialog = new PleaseWaitDialog(this);
    resultDialog = new ResultDialog(this);
    fileDialog = new QFileDialog(this);
    threadTimer = new QTimer(this);


    threadTimer->setSingleShot(false);

    connect( pleaseWaitDialog, SIGNAL(closePleasWaitDialog()), this, SLOT(onPleaseWaittDialogClose()) );
    connect( resultDialog, SIGNAL(closingDialog()), this, SLOT(onResultDialogClose()));
    connect( fileDialog, SIGNAL(currentChanged(QString)), this, SLOT(onFileSelected(QString)) );
    connect( threadTimer, SIGNAL(timeout()), this, SLOT(onTick()));

    ui->radioButton_SHA_1->setChecked(true);
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
    ui->lineEdit->setText(path);
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

        hashCalculatorVector.back()->init(*threadVector.back());
        hashCalculatorVector.back()->moveToThread(threadVector.back());
        threadVector.back()->start();

    }
    threadTimer->start(500);
    resultDialog->clearData();
    resultDialog->setAlgorithms(QCryptographicHash::Md5, QCryptographicHash::Sha1);
}

void MainWindow::calculateSingleChecksum(QCryptographicHash::Algorithm algoritm)
{
    hashCalculatorVector.push_back( new HashCalculator(this) );
    threadVector.push_back( new QThread(this) );

    hashCalculatorVector.back()->setFileName(fileName);
    hashCalculatorVector.back()->setHashAlgorithm(algoritm);
    hashCalculatorVector.back()->init(*threadVector.back());
    hashCalculatorVector.back()->moveToThread(threadVector.back());
    threadVector.back()->start();

    threadTimer->start(500);
    resultDialog->clearData();
    resultDialog->setAlgorithms(algoritm);
}


void MainWindow::on_pushButton_clicked()
{
    if(!calculating){

        hashCalculatorVector.clear();
        threadVector.clear();

        fileName = ui->lineEdit->text();
        if(fileName.size() == 0){
            QMessageBox::warning(this, "File Not Selected", "There is no file selected");
            return;
        }
        QFileInfo check(fileName);
        if(!check.exists() || !check.isFile()){
            QMessageBox::warning(this, "File does not exist!", fileName + " does not exist");
            return;
        }

        calculating = true;

        if(ui->radioButton_Both->isChecked()){
            calculate_MD5_and_SHA1();
        }

        if(ui->radioButton_MD4->isChecked()){
            calculateSingleChecksum(QCryptographicHash::Md4);
        }

        if(ui->radioButton_MD5->isChecked()){
            calculateSingleChecksum(QCryptographicHash::Md5);
        }

        if(ui->radioButton_SHA_1->isChecked()){
            calculateSingleChecksum(QCryptographicHash::Sha1);
        }

        if(ui->radioButton_SHA_224->isChecked()){
            calculateSingleChecksum(QCryptographicHash::Sha224);
        }

        if(ui->radioButton_SHA_256->isChecked()){
            calculateSingleChecksum(QCryptographicHash::Sha256);
        }

        if(ui->radioButton_SHA_384->isChecked()){
            calculateSingleChecksum(QCryptographicHash::Sha384);
        }

        if(ui->radioButton_SHA_512->isChecked()){
            calculateSingleChecksum(QCryptographicHash::Sha512);
        }

        if(ui->radioButton_SHA_3_224->isChecked()){
            calculateSingleChecksum(QCryptographicHash::Sha3_224);
        }

        if(ui->radioButton_SHA_3_256->isChecked()){
            calculateSingleChecksum(QCryptographicHash::Sha3_256);
        }

        if(ui->radioButton_SHA_3_384->isChecked()){
            calculateSingleChecksum(QCryptographicHash::Sha3_384);
        }

        if(ui->radioButton_SHA_3_512->isChecked()){
            calculateSingleChecksum(QCryptographicHash::Sha3_512);
        }

        pleaseWaitDialog->reinit();
        pleaseWaitDialog->show();
    }
}

void  MainWindow::onTick()
{
    foreach(QThread* thr, threadVector){
        if(thr->isRunning()){
            return;
        }
    }

    QString hashStrings[2];

    int iterator = 0;

    foreach(HashCalculator *hash, hashCalculatorVector){
        hashStrings[iterator].append( hash->hashValue.toHex() );
        iterator++;
    }

    pleaseWaitDialog->close();
    resultDialog->setChecksums(hashStrings[0], hashStrings[1]);
    resultDialog->show();
    threadTimer->stop();
}

void MainWindow::onResultDialogClose()
{
    resultDialog->clearData();
    calculating = false;
}

void MainWindow::onPleaseWaittDialogClose()
{
    hashCalculatorVector.clear();
    threadVector.clear();
    calculating = false;
}
