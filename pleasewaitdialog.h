#ifndef PLEASEWAITDIALOG_H
#define PLEASEWAITDIALOG_H

#include <QDialog>

namespace Ui {
class PleaseWaitDialog;
}

class PleaseWaitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PleaseWaitDialog(QWidget *parent = 0);
    ~PleaseWaitDialog();

private:
    Ui::PleaseWaitDialog *ui;
};

#endif // PLEASEWAITDIALOG_H
