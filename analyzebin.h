#ifndef ANALYZEBIN_H
#define ANALYZEBIN_H

#include "analyze.h"

class AnalyzeBin : public Analyze {
  public:
    AnalyzeBin(QString filepath);
    ~AnalyzeBin();
};

#endif // ANALYZEBIN_H
