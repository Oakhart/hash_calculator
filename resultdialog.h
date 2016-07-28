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

/**
 * @brief The ResultDialog class - Displays the checksum result/s in a dialog window.
 */
class ResultDialog : public QDialog
{
private:

    Q_OBJECT

    static const int noAlgorithm = 100; //!< value, used for indicating there is no second checksum

public:
    explicit ResultDialog(QWidget *parent = 0);
    ~ResultDialog();

    /**
     * @brief setChecksumResults - Sets the results to be displayed in the dialog line edits.
     * @param sum1 - the first checksum
     * @param sum2 - the second checksum (if any)
     */
    void setChecksumResults(QString sum1, QString sum2 = "");

    /**
     * @brief setAlgorithmLabels - Sets hash algorithm labels for the displayed checksums.
     * @param algorithm1 - the first algorithm
     * @param algorithm2 - the second algorithm (if any)
     */
    void setAlgorithmLabels(int algorithm1, int algorithm2 = noAlgorithm);

private slots:

    /**
     * @brief on_pushButton_clicked - Triggered when the "OK" button of the dialog is clicked
     * Closes the dialog window.
     */
    void on_pushButton_clicked();

private:
    Ui::ResultDialog*       ui;             //!< the dialog UI
    QVector<QLabel*>        labels;         //!< labels, indicating the hash algorithm for the displayed checksums
    QVector<QLineEdit*>     lineEdits;      //!< line edits, in which the checksums are displayed
    QVector<QVBoxLayout*>   vlayouts;       //!< vertical layouts for each pair of label and line edit
    QVBoxLayout*            dialogLayout;   //!< the dialog layout
    QHBoxLayout*            okBtnLayout;    //!< layout for the "OK" button
};

#endif // RESULTDIALOG_H
