#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QRadioButton>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // sets up the Main Window appearance (only the default widgets, and widgets added via graphical editor)
    ui->setupUi(this);

    // creates the "Please Wait" and "Result" dialogs
    pleaseWaitDialog = new PleaseWaitDialog(this);
    fileDialog = new QFileDialog(this);

    // creates the timer, checking for running threads
    threadTimer = new QTimer(this);

    // sets the timer in "pulse" mode
    threadTimer->setSingleShot(false);

    // connects the "Main Window" slots (those which are not connected via the graphical editor) to the corresponding signals
    connect( pleaseWaitDialog, SIGNAL(closePleasWaitDialog()), this, SLOT(onPleaseWaitDialogClose()) );
    connect( fileDialog, SIGNAL(currentChanged(QString)), this, SLOT(onFileSelected(QString)) );
    connect( threadTimer, SIGNAL(timeout()), this, SLOT(onTick()));

    // the calculating flag is set down, when ther Main Window is created
    calculating = false;

    // saves pointers to ui radio buttons in the vector
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
    // displays the file dialog
    fileDialog->show();
}

void MainWindow::onFileSelected(const QString & path)
{
    // when a file is selected in the file dialog, the path to file is loaded into the line edit
    // the path is not saved, because the user might want to edit it
    ui->lineEdit->setText(path);
}

void MainWindow::calculate_MD5_and_SHA1()
{
    //the following code runs once for MD5 and once for Sha1
    for(int i = 0; i < 2; ++i){

        // creates a new object, for caclculating a checksum
        hashCalculatorVector.push_back( new HashCalculator() );

        // creates a new thread, in which the object will do its calculations
        threadVector.push_back( new QThread(this) );

        // gives the calculating object the name of the file, which checksum will be calculated
        hashCalculatorVector.back()->setFileName(fileName);

        // sets MD5 or Sha1 hash algorithm for the calculation
        if(i == 0){
            hashCalculatorVector.back()->setHashAlgorithm(QCryptographicHash::Md5);
        } else {
            hashCalculatorVector.back()->setHashAlgorithm(QCryptographicHash::Sha1);
        }

        // connects the calculating object to the corresponding thread
        hashCalculatorVector.back()->init(*threadVector.back());

        // moves the calculation in the thread. Object (which inherits QObject) cannot be moved to a thread if its parent is pointed explicitly
        hashCalculatorVector.back()->moveToThread(threadVector.back());

        // starts the thread
        threadVector.back()->start();

    }

    // starts the timer, responsible for checking whether there are running threads
    threadTimer->start(500);

    // creates new Result dialog
    resultDialog = new ResultDialog();

    // sets the result labels according to the calculating algorithms
    resultDialog->setAlgorithms(QCryptographicHash::Md5, QCryptographicHash::Sha1);
}

void MainWindow::calculateSingleChecksum(QCryptographicHash::Algorithm algoritm)
{

    // creates a new object, for caclculating a checksum
    hashCalculatorVector.push_back( new HashCalculator() );

    // creates a new thread, in which the object will do its calculations
    threadVector.push_back( new QThread(this) );

    // gives the calculating object the name of the file, which checksum will be calculated
    hashCalculatorVector.back()->setFileName(fileName);

    // sets the algorithm for the calculation
    hashCalculatorVector.back()->setHashAlgorithm(algoritm);

    // connects the calculating object to the corresponding thread
    hashCalculatorVector.back()->init(*threadVector.back());

    // moves the calculation in the thread. Object (which inherits QObject) cannot be moved to a thread if its parent is pointed explicitly
    hashCalculatorVector.back()->moveToThread(threadVector.back());

    // starts the thread
    threadVector.back()->start();

    // starts the timer, responsible for checking whether there are running threads
    threadTimer->start(500);

    // creates new Result dialog
    resultDialog = new ResultDialog();

    // sets the result label according to the calculating algorithms
    resultDialog->setAlgorithms(algoritm);
}


void MainWindow::on_pushButton_clicked()
{
    // if there is a calculation running - do nothing
    if(calculating){
        return;
    }

    // deletes all existing calculating objects and threads
    hashCalculatorVector.clear();
    threadVector.clear();

    // gets the name of the file, which checksum/s the user want
    fileName = ui->lineEdit->text();

    // if the file name is empty string, display a warning box and return
    if(fileName.size() == 0){
        QMessageBox::warning(this, "File Not Selected", "There is no file selected");
        return;
    }

    // if there is no file with this name, display a warning box and return
    QFileInfo check(fileName);
    if(!check.exists() || !check.isFile()){
        QMessageBox::warning(this, "File Does Not Exist!", fileName + "\n does not exist");
        return;
    }

    // starts calculating if a single hash algorithm is selected
    for(int i = 0; i < SHA1_AND_MD5; ++i){
        if( radioButtons[i]->isChecked() ){
            calculateSingleChecksum( static_cast<QCryptographicHash::Algorithm>(i) );
        }
    }

    // starts calculating if the "MD5 and SHA1" option is selected
    if(radioButtons[SHA1_AND_MD5]->isChecked()){
        calculate_MD5_and_SHA1();
    }

    // resets the "Please Wait" dialog to initial state
    pleaseWaitDialog->reinit();

    // displays the "Please Wait" dialog
    pleaseWaitDialog->show();
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
