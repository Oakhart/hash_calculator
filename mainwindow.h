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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QFileDialog *fileDialog;

    QString fileName;

    PleaseWaitDialog *pleaseWaitDialog;
    ResultDialog    *resultDialog;

    QVector<HashCalculator*> hashCalculatorVector;
    QVector<QThread*> threadVector;

    QTimer *threadTimer;

    QVector<class QRadioButton*> radioButtons;
    class QGroupBox* groupBox;

    bool calculating;

    void calculate_MD5_and_SHA1();
    void calculateSingleChecksum(QCryptographicHash::Algorithm algoritm);

private slots:
    void onFileSelected(const QString & path);
    void onPleaseWaitDialogClose();
    void on_toolButton_clicked();
    void on_pushButton_clicked();
    void onTick();
};

#endif // MAINWINDOW_H
