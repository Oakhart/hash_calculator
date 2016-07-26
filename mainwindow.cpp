#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QRadioButton>

static QString radioButtonLabels[] = {

    "MD4",
    "MD5",
    "SHA-1"
    "SHA-224 (SHA-2)",
    "SHA-256 (SHA-2)",
    "SHA-384 (SHA-2)",
    "SHA-512 (SHA-2)",
    "SHA3-224",
    "SHA3-256",
    "SHA3-384",
    "SHA3-512",
    "MD5 and SHA-1"
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pleaseWaitDialog = new PleaseWaitDialog(this);
    fileDialog = new QFileDialog(this);
    threadTimer = new QTimer(this);


    threadTimer->setSingleShot(false);

    connect( pleaseWaitDialog, SIGNAL(closePleasWaitDialog()), this, SLOT(onPleaseWaitDialogClose()) );
    connect( fileDialog, SIGNAL(currentChanged(QString)), this, SLOT(onFileSelected(QString)) );
    connect( threadTimer, SIGNAL(timeout()), this, SLOT(onTick()));

    ui->radioButton_SHA_1->setChecked(true);
    calculating = false;

    radioButtons = {
      ui->radioButton_MD4,
      ui->radioButton_MD5,
      ui->radioButton_SHA_1,
      ui->radioButton_SHA_224,
      ui->radioButton_SHA_256,
      ui->radioButton_SHA_384,
      ui->radioButton_SHA_512,
      ui->radioButton_SHA_3_224,
      ui->radioButton_SHA_3_256,
      ui->radioButton_SHA_3_384,
      ui->radioButton_SHA_3_512,
      ui->radioButton_Both
    };

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

        hashCalculatorVector.push_back( new HashCalculator() );
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
        resultDialog = new ResultDialog();
    resultDialog->setAlgorithms(QCryptographicHash::Md5, QCryptographicHash::Sha1);
}

void MainWindow::calculateSingleChecksum(QCryptographicHash::Algorithm algoritm)
{
    hashCalculatorVector.push_back( new HashCalculator() );
    threadVector.push_back( new QThread(this) );

    hashCalculatorVector.back()->setFileName(fileName);
    hashCalculatorVector.back()->setHashAlgorithm(algoritm);
    hashCalculatorVector.back()->init(*threadVector.back());
    hashCalculatorVector.back()->moveToThread(threadVector.back());
    threadVector.back()->start();

    threadTimer->start(500);
    resultDialog = new ResultDialog();
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

        for(int i = 0; i < SHA1_AND_MD5; ++i){
            if( radioButtons[i]->isChecked() ){
                calculateSingleChecksum( static_cast<QCryptographicHash::Algorithm>(i) );
            }
        }

        if(radioButtons[SHA1_AND_MD5]->isChecked()){
            calculate_MD5_and_SHA1();
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
        ++iterator;
    }

    pleaseWaitDialog->close();
    calculating = false;
    resultDialog->setChecksums(hashStrings[0], hashStrings[1]);
    resultDialog->show();
    threadTimer->stop();
}

void MainWindow::onPleaseWaitDialogClose()
{
    hashCalculatorVector.clear();
    threadVector.clear();
    calculating = false;
    threadTimer->stop();
}
