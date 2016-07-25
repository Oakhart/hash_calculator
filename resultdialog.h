#ifndef RESULTDIALOG_H
#define RESULTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace Ui {
class ResultDialog;
}

class ResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResultDialog(QWidget *parent = 0);
    ~ResultDialog();

    void setChecksums(QString sum1, QString sum2 = "");
    void setAlgorithms(int algorithm1, int algorithm2 = 100);

private:
    Ui::ResultDialog *ui;

    QVector<QString> checkSums;
    QVector<QString> labelStrings;

    QVector<QLabel*> labels;
    QVector<QLineEdit*> lineEdits;
    QVector<QHBoxLayout*> hlayouts;
    QVBoxLayout *dialogLayout;
    QHBoxLayout* okBtnLayout;

    static const int invalidAlgorithm = 100;


};

#endif // RESULTDIALOG_H
