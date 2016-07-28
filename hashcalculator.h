#ifndef HASHCALCULATOR_H
#define HASHCALCULATOR_H

#include <QObject>
#include <QThread>
#include <QCryptographicHash>
#include <QTimer>

/**
 * @brief The HashCalculator class - Provides fuctionality to calculate checksums with different hash algorithms.
 */
class HashCalculator : public QObject
{
    Q_OBJECT
public:
    explicit HashCalculator(QObject *parent = 0);

    /**
     * @brief init - Connects a thread with the calculation process.
     * @param thread - the thread, in which the calculation work will be done
     */
    void init(QThread &thread);

    /**
     * @brief setFileName - Sets the full path to the file, which checksum will be calculated.
     * @param fileName - the full path to the file
     */
    void setFileName(QString fileName);

    /**
     * @brief setHashAlgorithm - Sets hashing algorithm for the checksum calculation.
     * @param hashAlgorithm - the algorithm to be used
     */
    void setHashAlgorithm(QCryptographicHash::Algorithm hashAlgorithm);

    /**
     * @brief disconnectFromThread - Disconnects calculator object slots from the thread signals.
     */
    void disconnectFromThread();

    /**
     * @brief getHashValue - Getter function for the result.
     * @return the result from the calculation
     */
    QByteArray getHashValueResult() const;


public slots:

    /**
     * @brief CalculateHash -Triggered when the connected thread start.
     * Caclulates the checksum of a file.
     */
    void CalculateHash();

private:
    QByteArray                      result;     //!< the result from the checsum calculation
    QString                         fileName;   //!< full path to the file, which checksum will be calculated
    QThread*                        thread;     //!< the thread, in which the calculation is running
    QCryptographicHash::Algorithm   algorithm;  //!< the algorithm, used for the checksum calculation
    class QMutex*                   mutex;      //!< mutex, used for locking the file, while calculation is running

    /**
     * @brief fileChecksum - The actual checksum calculation.
     * @param fileName - the file, which checksum will be calculated
     * @param hashAlgorithm - the hash algorithm to be used
     * @return the checksum in QByteArray format
     */
    QByteArray fileChecksum(const QString &fileName,
                            QCryptographicHash::Algorithm hashAlgorithm);
};

#endif // HASHCALCULATOR_H
