#ifndef PLEASEWAITDIALOG_H
#define PLEASEWAITDIALOG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class PleaseWaitDialog;
}

/**
 * @brief The PleaseWaitDialog class - Provides a dialog window, which is displayed while checsum calculation is running.
 * Also provides the functionality to cancel the calculation.
 */
class PleaseWaitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PleaseWaitDialog(QWidget *parent = 0);
    ~PleaseWaitDialog();

    /**
     * @brief reinit - Resets the dialog appearance to starting state
     */
    void reinit();

private:
    Ui::PleaseWaitDialog*   ui;                 //!< user interface for the dialog (holds all widgets, added via the graphical editor)
    QTimer*                 timer;              //!< timer, used for changing the progress bar value
    int                     progressBarValue;   //!< sets the progress bar appearance
    bool                    ascending;          //!< indicates whether the progress bar value is increasing or decreasing

private slots:

    /**
     * @brief onTick - Triggered when the timer emits timeout().
     * Changes the progress bar value
     */
    void onTick();

    /**
     * @brief on_pushButton_clicked - Triggered when the "Cancel" button is clicked.
     * Cancels the checksum calculation.
     */
    void on_pushButton_clicked();

signals:

    /**
     * @brief closePleasWaitDialog - Signal, emitted when the "Cancel" button is clicked.
     */
    void closePleasWaitDialog();
};

#endif // PLEASEWAITDIALOG_H
