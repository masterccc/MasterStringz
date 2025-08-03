#include "dataencoder.h"

#include <QRegExp>

DataEncoder::DataEncoder() : m_str_hex(""), m_str_bin(""), m_str_b64(""), m_str_ascii("") {}

void DataEncoder::Convert(EncodingType encodingType, QString data) {
    switch (encodingType) {
        case HEX:
            m_str_hex = data;
            break;
        case BINARY:
            m_str_bin = data;
            m_str_hex = fromByteArrayToHex(
                QByteArray::fromHex(fromByteArrayToBinary(data.toUtf8()).toUtf8()));
            break;
        case BASE64:
            m_str_b64 = data;
            m_str_hex = fromByteArrayToHex(QByteArray::fromBase64(data.toUtf8()));
            break;
        case ASCII:
            m_str_ascii = data;
            m_str_hex   = fromByteArrayToHex(data.toUtf8());
            break;
        default:
            break;
    }

    m_str_bin   = fromByteArrayToBinary(fromHexToByteArray(m_str_hex).toHex());
    m_str_b64   = QByteArray::fromHex(m_str_hex.toUtf8()).toBase64();
    m_str_ascii = fromHexToByteArray(m_str_hex);
}

QString DataEncoder::str_hex() const {
    return m_str_hex;
}

QString DataEncoder::str_bin() const {
    return m_str_bin;
}

QString DataEncoder::str_b64() const {
    return m_str_b64;
}

QString DataEncoder::str_ascii() const {
    return m_str_ascii;
}

QByteArray DataEncoder::fromHexToByteArray(QString hexData) {
    hexData.remove(QRegExp("\\s+"));
    return QByteArray::fromHex(hexData.toUtf8());
}

QString DataEncoder::fromByteArrayToHex(QByteArray ba) {
    return QString::fromUtf8(ba.toHex());
}

QString DataEncoder::fromByteArrayToBinary(QByteArray ba) {
    QString binaryString = "";

    for (int i = 0; i < ba.length(); i++) {
        QString byteString = QString::number(ba.at(i), 2);
        byteString         = byteString.rightJustified(8, '0');
        binaryString.append(byteString);
    }

    return binaryString;
}

QString DataEncoder::fromByteArrayToBase64(QByteArray ba) {
    return ba.toBase64();
}

QString DataEncoder::fromHexToASCII(QString hexData) {
    hexData.remove(QRegExp("\\s+"));
    QByteArray bytes     = QByteArray::fromHex(hexData.toUtf8());
    QString    asciiData = QString::fromUtf8(bytes);
    return asciiData;
}
