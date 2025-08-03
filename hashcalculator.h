#ifndef HASHCALCULATOR_H
#define HASHCALCULATOR_H

#include <QString>
#include <QByteArray>
#include <QFile>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

class HashCalculator {
  public:
    HashCalculator(QString filename);
    ~HashCalculator();

    bool calculateHashes(const QString& fileName);

    QString getMD5() const;
    QString getSHA512() const;
    QString getSHA256() const;
    QString getSSDeep() const;

  private:
    QString m_md5;
    QString m_sha512;
    QString m_sha256;
    QString m_ssdeep;

    void calculateMD5(const unsigned char* data, size_t length);
    void calculateSHA512(const unsigned char* data, size_t length);
    void calculateSHA256(const unsigned char* data, size_t length);
    void calculateSSDeep(const unsigned char* data, size_t length);
};

#endif // HASHCALCULATOR_H
