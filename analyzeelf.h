#ifndef ANALYZEELF_H
#define ANALYZEELF_H

#include "analyze.h"

class AnalyzeElf : public Analyze {
  public:
    AnalyzeElf(QString filepath);
    ~AnalyzeElf();
    QMap<QString, QString>*     ParseHeader() override;
    QVector<BinarySection>*     GetSections() override;
    QVector<QVector<QString>*>* GetImports() override;
};

#endif // ANALYZEELF_H
