#include "resultdialog.h"
#include "ui_resultdialog.h"
#include "QDebug"

//array of the possible label texts
static const QString resultLabels[] = {

    "MD4 Checksum: ",
    "MD5 Checksum: ",
    "SHA-1 Checksum: ",
    "SHA-224 (SHA-2) Checksum: ",
    "SHA-256 (SHA-2) Checksum: ",
    "SHA-384 (SHA-2) Checksum: ",
    "SHA-512 (SHA-2) Checksum: ",
    "SHA3-224 Checksum: ",
    "SHA3-256 Checksum: ",
    "SHA3-384 Checksum: ",
    "SHA3-512 Checksum: ",

};

ResultDialog::ResultDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultDialog)
{
    ui->setupUi(this);

    // resizes the dialog
    this->resize(700,160);

}

ResultDialog::~ResultDialog()
{
    delete ui;
}

void ResultDialog::setChecksumResults(QString sum1, QString sum2)
{
    QString sums[2] = { sum1, sum2 };

    // for each checksum, if the checksum string != 0, sets up the string value to be displayed in a read only line edit
    for(int i = 0; i < 2; ++i ){
        if(sums[i].size() != 0){
            lineEdits.push_back(new QLineEdit());
            lineEdits.back()->setReadOnly(true);
            lineEdits.back()->setText(sums[i]);
            lineEdits.back()->setVisible(true);

            // forms a layout out of the line edit, and the corresponding label
            vlayouts.push_back(new QVBoxLayout());
            vlayouts.back()->addWidget(labels[i]);
            vlayouts.back()->addWidget(lineEdits[i]);
        }
    }

    // creates the dialog layout
    dialogLayout = new QVBoxLayout(this);

    // fils the dialog layout with all the layouts for the results
    for(int i = 0; i < vlayouts.size(); ++i){
        dialogLayout->insertLayout(-1, vlayouts[i]);

    }

    // creates a layout for the "OK" button
    okBtnLayout = new QHBoxLayout();
    okBtnLayout->addStretch();
    okBtnLayout->addWidget(ui->pushButton);
    okBtnLayout->addStretch();

    // inserts the "OK" button layout into the dialog layout
    dialogLayout->addStretch(1);
    dialogLayout->insertLayout(-1, okBtnLayout);
}

void ResultDialog::setAlgorithmLabels(int algorithm1, int algorithm2 )
{
    int algorithms[2] = {algorithm1, algorithm2};

    // for each algorithm passed as an argulent, creates a label. If the algorithm is noAlgorithm - does nothing
    for(int i = 0; i < 2; ++i ){
        if(algorithms[i] != noAlgorithm){
            labels.push_back(new QLabel());
            labels.back()->setText(resultLabels[algorithms[i]]);
        }
    }
}

void ResultDialog::on_pushButton_clicked()
{
    // closes the dialog
    this->close();
    this->deleteLater();
}

