#ifndef ANALYZE_H
#define ANALYZE_H

#include <QElapsedTimer>
#include <QString>

#include "binarysection.h"
enum FILETYPE { ELF, OTHER };

class Analyze {
  public:
    Analyze();
    virtual ~Analyze();
    Analyze(QString filepath);
    static enum FILETYPE                GetFileType(QString filepath);
    QMap<QString, QString>*             GetNtHeader();
    qint64                              getDuration() const;
    void                                setDuration(qint64 newDuration);
    QString                             GetAnalyzeDuration();
    QStringList*                        strs;
    QVector<QString>*                   GetStrings(bool utf8, bool utf16);
    virtual QMap<QString, QString>*     ParseHeader();
    virtual QVector<BinarySection>*     GetSections();
    virtual QVector<QVector<QString>*>* GetImports();

    // QVector<PEImport> *GetImports();
  protected:
    QString                 filepath;
    QVector<QString>*       extracted_strings;
    QVector<BinarySection>* sections;
    QElapsedTimer           timer;
    qint64                  duration;
};

#endif // ANALYZE_H
