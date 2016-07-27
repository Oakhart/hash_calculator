#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include "hashcalculator.h"
#include "pleasewaitdialog.h"
#include "resultdialog.h"

namespace Ui {
class MainWindow;
}

// indexes for the vector, containing the radioButton pointers
enum radioButtonIndexes{
    MD4,
    MD5,
    SHA1,
    SHA_224,
    SHA_256,
    SHA_384,
    SHA_512,
    SHA_3_224,
    SHA_3_256,
    SHA_3_384,
    SHA_3_512,
    SHA1_AND_MD5,
    COUNT
};

/**
 *@brief The application main window.
 */
class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    Ui::MainWindow*             ui;                     //!< user interface for the main window
    QFileDialog*                fileDialog;             //!< file dialog, opens when the "Select File..." button is clicked
    QString                     fileName;               //!< full path to the file, which checksum/s will be calculated
    PleaseWaitDialog*           pleaseWaitDialog;       //!< dialog, indicating the checksum calculation is running at the moment
    ResultDialog*               resultDialog;           //!< dialog, displaying the checksum
    QVector<HashCalculator*>    hashCalculatorVector;   //!< objects for calculating checksums. Each object calculates only one checksum, and runs in different thread
    QVector<QThread*>           threadVector;           //!< vector of threads, in which HashCalculator objects are calculating
    QVector<class QRadioButton*>radioButtons;           //!< vector of pointers to all radio buttons in the interface ( for easier access )
    QVector<HashCalculator*>    calculatorsForDeletion; //!< vector, holding calculator objects, scheduled for deletion, after their corresponding threads exit
    QVector<QThread*>           threadsForDeletion;     //!< vector, holding threads scheduled for deletion
    QTimer*                     clearingTimer;          //!< timer responsible for deleting scheduled for deletion objects from calculatorsForDeletion and threadsForDeletion
    QTimer*                     threadTimer;            //!< timer, which starts the check for running threads
    bool calculating;                                   //!< flag, indicating whether a calculation is running. If it's up, "Calculate Hash" button is inactive.

    /**
     * @brief calculate_MD5_and_SHA1 - Calculates both MD5 and SHA1 checksums, and displays them in a single ResultDialog
     */
    void calculate_MD5_and_SHA1();

    /**
     * @brief calculateSingleChecksum - Calculates a single checksum, and displays the result in a ResultDialog
     *
     * @param algoritm - The algorithm to be used for the hash calculation
     */
    void calculateSingleChecksum(QCryptographicHash::Algorithm algoritm);

    /**
     * @brief deleteThreads - Schedules all current threads for deletion, and clears the vector with current threads
     */
    void deleteThreads();

    /**
     * @brief deleteHashcalculators - Schedules all current calculating objects for deletion, and clears the vector with current calculating objects
     */
    void deleteHashcalculators();

    /**
     * @brief clearAfterCalculation - Clears both calculating objects and threads and starts a timer, responsible for deleting them
     */
    void clearAfterCalculation();


private slots:

    /**
     * @brief onFileSelected - Triggered when a file is selected from the file dialog.
     * Adds the full path to the selected file to the line edit of the Main Window.
     *
     * @param path - full path to the selected file,
     */
    void onFileSelected(const QString & path);

    /**
     * @brief onPleaseWaitDialogClose - Triggered when the "Cancel" button of the "Please Wait" dialog box is pressed.
     * Aborts the checksum calculation.
     */
    void onPleaseWaitDialogClose();

    /**
     * @brief on_toolButton_clicked - Triggered when the "Select File..." button is clicked.
     * Opens the file dialog.
     */
    void on_toolButton_clicked();

    /**
     * @brief on_pushButton_clicked - Triggered when the "Calculate Hash" button is clicked;
     * Starts calculating the checksum/s for the corresponding radio button
     */
    void on_pushButton_clicked();

    /**
     * @brief onTick - Triggered when the threadTimer emits timeout()
     * Checks for running threads, and if there are none, displays the result of the calculation.
     */
    void onTick();

    /**
     * @brief onTickClear - Triggered when the clearTimer emits timeout()
     * Deletes all threads and calculating objects from past calculations (if threads are not running)
     */
    void onTickClear();

};

#endif // MAINWINDOW_H
