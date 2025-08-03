#ifndef DATAENCODER_H
#define DATAENCODER_H

#include <QString>

// Enumération pour les différents types d'encodage
enum EncodingType { HEX, BINARY, BASE64, ASCII };

class DataEncoder {
  public:
    DataEncoder();

    // Méthode de conversion
    void Convert(EncodingType encodingType, QString data);

    // Accesseurs pour les chaînes de caractères encodées
    QString str_hex() const;
    QString str_bin() const;
    QString str_b64() const;
    QString str_ascii() const;

  private:
    // Variables pour les chaînes de caractères encodées
    QString m_str_hex;
    QString m_str_bin;
    QString m_str_b64;
    QString m_str_ascii;

    // Fonctions de conversion privées
    QByteArray fromHexToByteArray(QString hexData);
    QString    fromByteArrayToHex(QByteArray ba);
    QString    fromByteArrayToBinary(QByteArray ba);
    QString    fromByteArrayToBase64(QByteArray ba);
    QString    fromHexToASCII(QString hexData);
};

#endif // DATAENCODER_H
