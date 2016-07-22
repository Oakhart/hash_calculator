#ifndef HASHCALCULATOR_H
#define HASHCALCULATOR_H

#include <QObject>

class HashCalculator : public QObject
{
    Q_OBJECT
public:
    explicit HashCalculator(QObject *parent = 0);

signals:

public slots:
};

#endif // HASHCALCULATOR_H
