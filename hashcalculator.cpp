#include "hashcalculator.h"
#include <QFile>
#include <QDebug>
#include <QMutex>

HashCalculator::HashCalculator(QObject *parent) : QObject(parent)
{

}

void HashCalculator::init(QThread &thread)
{
    // saves a pointer to the thread for further manipulation
    this->thread = &thread;

    // connects the thread signal to the calculator object calculation
    connect( &thread, SIGNAL(started()), this, SLOT(CalculateHash()) );
}

void HashCalculator::CalculateHash()
{
    // calculates the checksum of a file, saves the result, and stops the thread from running
    result = fileChecksum(fileName, algorithm);
    thread->quit();
}

void HashCalculator::setFileName(QString fileName)
{
    //sets the full path to the file, which checksum will be calculated
    this->fileName = fileName;
}

void HashCalculator::setHashAlgorithm(QCryptographicHash::Algorithm hashAlgorithm)
{
    // sets the algorithm, which will be used for the checksum calculation
    this->algorithm = hashAlgorithm;
}

QByteArray HashCalculator::fileChecksum(const QString &fileName,
                        QCryptographicHash::Algorithm hashAlgorithm)
{
    // locks the file, so it cannot be modified while the calculation is running
    mutex = new QMutex();
    mutex->lock();

    // if the file exists, and the checksum can be calculated, returns the checksum
    QFile f(fileName);
    if (f.open(QFile::ReadOnly)) {

        QCryptographicHash hash(hashAlgorithm);
        if (hash.addData(&f)) {
            // unlocks the file for modiying
            mutex->unlock();
            return hash.result();
        }
    }

    // unlocks the file for modifying, and returns an empty QByteArray
    mutex->unlock();
    return QByteArray();
}

void HashCalculator::disconnectFromThread()
{
    //disconnects thread signal from hash calculator object slot
    disconnect( thread, SIGNAL(started()), this, SLOT(CalculateHash()) );
}

QByteArray HashCalculator::getHashValueResult() const
{
    return result;
}
