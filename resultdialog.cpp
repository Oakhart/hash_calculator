#include "resultdialog.h"
#include "ui_resultdialog.h"
#include "QDebug"

ResultDialog::ResultDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultDialog)
{
    ui->setupUi(this);

}

ResultDialog::~ResultDialog()
{
    delete ui;
}

void ResultDialog::setChecksums(QString sum1, QString sum2)
{
    QString sums[2];
    sums[0] = sum1;
    sums[1] = sum2;

    for(int i = 0; i < 2; ++i ){
        if(sums[i].size() != 0){
            lineEdits.push_back(new QLineEdit());
            lineEdits.back()->setReadOnly(true);
            lineEdits.back()->setText(sums[i]);
            lineEdits.back()->setVisible(true);

            hlayouts.push_back(new QHBoxLayout());
            hlayouts.back()->addWidget(labels[i]);
            hlayouts.back()->addWidget(lineEdits[i]);

        }
    }

    dialogLayout = new QVBoxLayout(this);

    for(int i = 0; i < hlayouts.size(); ++i){
        dialogLayout->insertLayout(-1, hlayouts[i]);

    }
    okBtnLayout = new QHBoxLayout();
    okBtnLayout->addStretch();
    okBtnLayout->addWidget(ui->pushButton);
        okBtnLayout->addStretch();

    dialogLayout->insertLayout(-1, okBtnLayout);
}

void ResultDialog::setAlgorithms(int algorithm1, int algorithm2 )
{
    int algorithms[2];
    algorithms[0] = algorithm1;
    algorithms[1] = algorithm2;

    for(int i = 0; i < 2; ++i ){
        if(algorithms[i] != invalidAlgorithm){
            labels.push_back(new QLabel());
        }

        switch(algorithms[i]){

            case 0:{ labels.back()->setText("MD4 Checksum: "); break; }

            case 1:{ labels.back()->setText("MD5 Checksum: "); break; }

            case 2:{ labels.back()->setText("SHA-1 Checksum: "); break; }

            case 3:{ labels.back()->setText("SHA-224 (SHA-2) Checksum: "); break; }

            case 4:{ labels.back()->setText("SHA-256 (SHA-2) Checksum: "); break; }

            case 5:{ labels.back()->setText("SHA-384 (SHA-2) Checksum: "); break; }

            case 6:{ labels.back()->setText("SHA-512 (SHA-2) Checksum: "); break; }

            case 7:{ labels.back()->setText("SHA3-224 Checksum: "); break; }

            case 8:{ labels.back()->setText("SHA3-256 Checksum: "); break; }

            case 9:{ labels.back()->setText("SHA3-384 Checksum: "); break; }

            case 10:{ labels.back()->setText("SHA3-512 Checksum: "); break; }

            default: { break;}
        }
    }
}

void ResultDialog::clearData()
{
    labels.clear();
    lineEdits.clear();
    hlayouts.clear();
}

void ResultDialog::on_pushButton_clicked()
{
    emit closingDialog();
    this->close();
}

