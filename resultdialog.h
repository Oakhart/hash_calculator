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
    void clearData();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ResultDialog *ui;

    QVector<QLabel*> labels;
    QVector<QLineEdit*> lineEdits;
    QVector<QHBoxLayout*> hlayouts;
    QVBoxLayout *dialogLayout;
    QHBoxLayout* okBtnLayout;

    static const int invalidAlgorithm = 100;

    void clearLayout(QLayout* layout, bool deleteWidgets = true);



};

#endif // RESULTDIALOG_H
