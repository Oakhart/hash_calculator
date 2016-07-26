#include "resultdialog.h"
#include "ui_resultdialog.h"
#include "QDebug"

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

    this->resize(700,160);

}

ResultDialog::~ResultDialog()
{
    delete ui;
}

void ResultDialog::setChecksums(QString sum1, QString sum2)
{
    QString sums[2] = { sum1, sum2 };

    for(int i = 0; i < 2; ++i ){
        if(sums[i].size() != 0){
            lineEdits.push_back(new QLineEdit());
            lineEdits.back()->setReadOnly(true);
            lineEdits.back()->setText(sums[i]);
            lineEdits.back()->setVisible(true);

            vlayouts.push_back(new QVBoxLayout());
            vlayouts.back()->addWidget(labels[i]);
            vlayouts.back()->addWidget(lineEdits[i]);
        }
    }

    dialogLayout = new QVBoxLayout(this);

    for(int i = 0; i < vlayouts.size(); ++i){
        dialogLayout->insertLayout(-1, vlayouts[i]);

    }
    okBtnLayout = new QHBoxLayout();
    okBtnLayout->addStretch();
    okBtnLayout->addWidget(ui->pushButton);
    okBtnLayout->addStretch();
    dialogLayout->addStretch(1);
    dialogLayout->insertLayout(-1, okBtnLayout);
}

void ResultDialog::setAlgorithms(int algorithm1, int algorithm2 )
{
    int algorithms[2] = {algorithm1, algorithm2};

    for(int i = 0; i < 2; ++i ){
        if(algorithms[i] != invalidAlgorithm){
            labels.push_back(new QLabel());
            labels.back()->setText(resultLabels[algorithms[i]]);
        }
    }
}

void ResultDialog::on_pushButton_clicked()
{
    this->close();
    this->deleteLater();
}

