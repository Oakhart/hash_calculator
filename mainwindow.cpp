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
    clearingTimer = new QTimer(this);

    // sets the timer in "pulse" mode
    threadTimer->setSingleShot(false);
    clearingTimer->setSingleShot(false);

    // connects the "Main Window" slots (those which are not connected via the graphical editor) to the corresponding signals
    connect( pleaseWaitDialog, SIGNAL(closePleasWaitDialog()), this, SLOT(onPleaseWaitDialogClose()) );
    connect( fileDialog, SIGNAL(currentChanged(QString)), this, SLOT(onFileSelected(QString)) );
    connect( threadTimer, SIGNAL(timeout()), this, SLOT(onTick()) );
    connect( clearingTimer, SIGNAL(timeout()), this, SLOT(onTickClear()) );

    // the calculating flag is set down, when ther Main Window is created
    calculating = false;


    clearingTimer->start(200);

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

    radioButtons[SHA1]->setChecked(true);
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
        threadVector.push_back( new QThread() );

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
    resultDialog->setAlgorithmLabels(QCryptographicHash::Md5, QCryptographicHash::Sha1);
}

void MainWindow::calculateSingleChecksum(QCryptographicHash::Algorithm algoritm)
{

    // creates a new object, for caclculating a checksum
    hashCalculatorVector.push_back( new HashCalculator() );

    // creates a new thread, in which the object will do its calculations
    threadVector.push_back( new QThread() );

    // gives the calculating object the name of the file, which checksum will be calculated
    hashCalculatorVector.back()->setFileName(fileName);

    // sets the algorithm for the calculation, moves the calculating object into the thread, and starts it(the thread)
    hashCalculatorVector.back()->setHashAlgorithm(algoritm);
    hashCalculatorVector.back()->init(*threadVector.back());
    hashCalculatorVector.back()->moveToThread(threadVector.back());
    threadVector.back()->start();

    // starts the timer, responsible for checking whether there are running threads
    threadTimer->start(500);

    // creates new Result dialog
    resultDialog = new ResultDialog();

    // sets the result label according to the calculating algorithms
    resultDialog->setAlgorithmLabels(algoritm);
}


void MainWindow::on_pushButton_clicked()
{
    // if there is a calculation running - do nothing
    if(calculating){
        return;
    }

    // deletes all existing calculating objects and threads
    clearAfterCalculation();
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

    // resets the "Please Wait" dialog to initial state and displays it
    pleaseWaitDialog->reinit();
    pleaseWaitDialog->show();
}

void  MainWindow::onTick()
{
    // if there is a running thread - do nothing
    foreach(QThread* thr, threadVector){
        if(thr->isRunning()){
            return;
        }
    }

    QString hashStrings[2];
    int iterator = 0;

    // gets the result from every calculating object
    foreach(HashCalculator *hash, hashCalculatorVector){
        hashStrings[iterator].append( hash->getHashValueResult().toHex() );
        ++iterator;
    }

    // closes the "Please Wait" dialog, and resets the calculating flag, the timer no longer needs to run
    pleaseWaitDialog->close();
    calculating = false;
    threadTimer->stop();

    // the checksums are given to the "Result" dialog, and it is displayed
    resultDialog->setChecksumResults(hashStrings[0], hashStrings[1]);
    resultDialog->show();
}

void MainWindow::onPleaseWaitDialogClose()
{
    // when the "Please Wait" dialog "Cancel" button is clicked, all threads and calculating objects are deleted, calculation stops
    clearAfterCalculation();
    calculating = false;
    threadTimer->stop();
}

void MainWindow::deleteThreads()
{
    // moves all items to the vector of items scheduled for deletion, and clears the vector used during calculation
    for(int i = 0; i < threadVector.size(); ++i)
    {
        threadsForDeletion.push_back(threadVector[i]);
    }

    threadVector.clear();
}

void MainWindow::deleteHashcalculators()
{
    // moves all items to the vector of items scheduled for deletion, and clears the vector used during calculation
    for(int i = 0; i < hashCalculatorVector.size(); ++i)
    {
        calculatorsForDeletion.push_back(hashCalculatorVector[i]);
    }

    hashCalculatorVector.clear();
}

void MainWindow::onTickClear()
{

    for(int i = 0; i < threadsForDeletion.size(); ++i){

        //if the thread is running - do nothing
        if(threadsForDeletion[i]->isRunning()){
            continue;
        }

        // creates pointers for the items to be deleted
        QThread* thread = threadsForDeletion[i];
        HashCalculator* calculator = calculatorsForDeletion[i];

        // removes items from the vectors
        calculatorsForDeletion.removeAt(i);
        threadsForDeletion.removeAt(i);

        // disconnects the calculator object slots from the thread signals
        calculator->disconnectFromThread();

        // frees the memory these objects are taking
        delete thread;
        delete calculator;
    }

    // if nothing is scheduled for deletion, timer no longer needs to run
    if(threadsForDeletion.isEmpty()){
        if(clearingTimer->isActive()){
            clearingTimer->stop();
        }
    }
}

void MainWindow::clearAfterCalculation()
{
    // when the calculation is cancelled, or a new calculation start, the data from the previous calculation needs to be wiped out
    deleteThreads();
    deleteHashcalculators();
    clearingTimer->start(200);
}
