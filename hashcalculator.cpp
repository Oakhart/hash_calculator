#include "hashcalculator.h"
#include <QFile>
#include <QDebug>

HashCalculator::HashCalculator(QObject *parent) : QObject(parent)
{

}

void HashCalculator::init(QThread &thread)
{
    this->thread = &thread;
    connect( &thread, SIGNAL(started()), this, SLOT(CalculateHash()) );
}

void HashCalculator::CalculateHash()
{
    hashValue = fileChecksum(fileName, algorithm);
    thread->exit();
}

void HashCalculator::setFileName(QString fileName)
{
    this->fileName = fileName;
}

void HashCalculator::setHashAlgorithm(QCryptographicHash::Algorithm hashAlgorithm)
{
    this->algorithm = hashAlgorithm;
}

QByteArray HashCalculator::fileChecksum(const QString &fileName,
                        QCryptographicHash::Algorithm hashAlgorithm)
{
    QFile f(fileName);
    if (f.open(QFile::ReadOnly)) {
        QCryptographicHash hash(hashAlgorithm);
        if (hash.addData(&f)) {
            return hash.result();
        }
    }
    return QByteArray();
}
