#include "hashcalculator.h"

#include <fuzzy.h>
#include <stdexcept>

HashCalculator::HashCalculator(QString filename) {
    SSL_library_init();
    OpenSSL_add_all_digests();
    calculateHashes(filename);
}

HashCalculator::~HashCalculator() {
    EVP_cleanup();
}

bool HashCalculator::calculateHashes(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    // Determine file size
    qint64 fileSize = file.size();

    // Allocate buffer for file contents
    QByteArray fileContents = file.readAll();

    // Calculate hashes
    calculateMD5((const unsigned char*) fileContents.constData(), fileContents.size());
    calculateSHA512((const unsigned char*) fileContents.constData(), fileContents.size());
    calculateSHA256((const unsigned char*) fileContents.constData(), fileContents.size());
    calculateSSDeep((const unsigned char*) fileContents.constData(), fileContents.size());

    return true;
}

QString HashCalculator::getMD5() const {
    return m_md5;
}

QString HashCalculator::getSHA512() const {
    return m_sha512;
}

QString HashCalculator::getSHA256() const {
    return m_sha256;
}

QString HashCalculator::getSSDeep() const {
    return m_ssdeep;
}

void HashCalculator::calculateMD5(const unsigned char* data, size_t length) {
    unsigned char md5Digest[MD5_DIGEST_LENGTH];
    MD5(data, length, md5Digest);

    QString result;
    result.reserve(MD5_DIGEST_LENGTH * 2);
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        result.append(QString::number(md5Digest[i], 16).rightJustified(2, '0'));
    }
    m_md5 = result;
}

void HashCalculator::calculateSHA512(const unsigned char* data, size_t length) {
    unsigned char sha512Digest[SHA512_DIGEST_LENGTH];
    SHA512(data, length, sha512Digest);

    QString result;
    result.reserve(SHA512_DIGEST_LENGTH * 2);
    for (int i = 0; i < SHA512_DIGEST_LENGTH; i++) {
        result.append(QString::number(sha512Digest[i], 16).rightJustified(2, '0'));
    }
    m_sha512 = result;
}

void HashCalculator::calculateSHA256(const unsigned char* data, size_t length) {
    unsigned char sha256Digest[SHA256_DIGEST_LENGTH];
    SHA256(data, length, sha256Digest);

    QString result;
    result.reserve(SHA256_DIGEST_LENGTH * 2);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        result.append(QString::number(sha256Digest[i], 16).rightJustified(2, '0'));
    }
    m_sha256 = result;
}

void HashCalculator::calculateSSDeep(const unsigned char* data, size_t length) {
    unsigned char sha1Digest[SHA_DIGEST_LENGTH];
    SHA1(data, length, sha1Digest);

    char ssdeepDigest[4096];
    memset(ssdeepDigest, 0, sizeof(ssdeepDigest));
    int ssdeepResult = fuzzy_hash_buf((const unsigned char*) data, length, ssdeepDigest);
    if (ssdeepResult == 0) {
        m_ssdeep = QString::fromUtf8(ssdeepDigest);
    } else {
        // qWarning() << "Failed to calculate SSDeep hash";
    }
}
