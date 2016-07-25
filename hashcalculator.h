#ifndef HASHCALCULATOR_H
#define HASHCALCULATOR_H

#include <QObject>
#include <QThread>
#include <QCryptographicHash>
#include <QTimer>

class HashCalculator : public QObject
{
    Q_OBJECT
public:
    explicit HashCalculator(QObject *parent = 0);
    void init(QThread &thread);
    void setFileName(QString fileName);
    void setHashAlgorithm(QCryptographicHash::Algorithm hashAlgorithm);

    QByteArray hashValue;


public slots:
    void CalculateHash();

signals:


private:
    QString fileName;
    QThread *thread;
    QCryptographicHash::Algorithm algorithm;

    QByteArray fileChecksum(const QString &fileName,
                            QCryptographicHash::Algorithm hashAlgorithm);
};

#endif // HASHCALCULATOR_H
