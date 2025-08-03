#include "analyzebin.h"
#include <elf.h>
#include <fcntl.h>
#include <libelf.h>


AnalyzeBin::AnalyzeBin(QString filepath) {
    this->filepath = filepath;
}

AnalyzeBin::~AnalyzeBin() {}
