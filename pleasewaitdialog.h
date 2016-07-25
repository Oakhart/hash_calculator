#ifndef PLEASEWAITDIALOG_H
#define PLEASEWAITDIALOG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class PleaseWaitDialog;
}

class PleaseWaitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PleaseWaitDialog(QWidget *parent = 0);
    ~PleaseWaitDialog();

    void reinit();
private:
    Ui::PleaseWaitDialog *ui;
    QTimer *timer;
    int progressBarValue;
    bool ascending;

private slots:
    void onTick();
    void on_pushButton_clicked();

signals:
    void closePleasWaitDialog();
};

#endif // PLEASEWAITDIALOG_H
