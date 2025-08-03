#ifndef BINARYSECTION_H
#define BINARYSECTION_H

#include <QBitArray>
#include <QString>

class BinarySection {
  public:
    BinarySection(QString name = "", qint64 offset = 0, qint64 virtualSize = 0,
                  QBitArray permissions = QBitArray(3), double entropy = 0.0, QString comment = "");

    QString GetName() const;
    void    SetName(QString name);

    qint64 GetOffset() const;
    void   SetOffset(qint64 offset);

    qint64 GetVirtualSize() const;
    void   SetVirtualSize(qint64 virtualSize);

    QBitArray GetPermissions() const;
    void      SetPermissions(QBitArray permissions);

    double GetEntropy() const;
    void   SetEntropy(double entropy);

    QString GetComment() const;
    void    SetComment(QString comment);

  private:
    QString   m_name;
    qint64    m_offset;
    qint64    m_virtualSize;
    QBitArray m_permissions;
    double    m_entropy;
    QString   m_comment;
};

#endif // BINARYSECTION_H
