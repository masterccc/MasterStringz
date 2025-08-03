#include "binarysection.h"

BinarySection::BinarySection(QString name, qint64 offset, qint64 virtualSize, QBitArray permissions,
                             double entropy, QString comment)
    : m_name(name), m_offset(offset), m_virtualSize(virtualSize), m_permissions(permissions),
      m_entropy(entropy), m_comment(comment) {}

QString BinarySection::GetName() const {
    return m_name;
}

void BinarySection::SetName(QString name) {
    m_name = name;
}

qint64 BinarySection::GetOffset() const {
    return m_offset;
}

void BinarySection::SetOffset(qint64 offset) {
    m_offset = offset;
}

qint64 BinarySection::GetVirtualSize() const {
    return m_virtualSize;
}

void BinarySection::SetVirtualSize(qint64 virtualSize) {
    m_virtualSize = virtualSize;
}

QBitArray BinarySection::GetPermissions() const {
    return m_permissions;
}

void BinarySection::SetPermissions(QBitArray permissions) {
    m_permissions = permissions;
}

double BinarySection::GetEntropy() const {
    return m_entropy;
}

void BinarySection::SetEntropy(double entropy) {
    m_entropy = entropy;
}

QString BinarySection::GetComment() const {
    return m_comment;
}

void BinarySection::SetComment(QString comment) {
    m_comment = comment;
}
