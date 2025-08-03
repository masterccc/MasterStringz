#include "analyze.h"

#include <QFile>
#include <QString>
#include <QStringList>
#include <QVector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#define MIN_STRING_LEN 5
#define is_char(x) ((x == 9) || ((x >= 32) && (x <= 126)))
#define is_printable_string(str)                                                                   \
    (std::all_of(str.begin(), str.end(), [](char c) { return is_char(c); }))

Analyze::Analyze() {
    this->timer.start();
    this->extracted_strings = new QVector<QString>();
}

Analyze::~Analyze() {}

Analyze::Analyze(QString filepath) {
    this->filepath = filepath;
    this->timer.start();
    this->extracted_strings = new QVector<QString>();
}

QMap<QString, QString>* Analyze::ParseHeader() {}
QVector<BinarySection>*     Analyze::GetSections() {}
QVector<QVector<QString>*>* Analyze::GetImports() {}

enum FILETYPE Analyze::GetFileType(QString filepath) {
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly)) {
        return OTHER;
    }
    char magic[4];
    if (file.read(magic, 4) != 4) {
        file.close();
        return OTHER;
    }
    file.close();

    if (magic[0] == 0x7f && magic[1] == 'E' && magic[2] == 'L' && magic[3] == 'F') {
        return ELF;
    } else {
        return OTHER;
    }
}

QVector<QString>* Analyze::GetStrings(bool utf8, bool utf16) {
    if (!this->extracted_strings->empty()) return this->extracted_strings;

    QVector<QString>* result = new QVector<QString>();

    std::ifstream file(this->filepath.toStdString(), std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file \n";
    }

    std::string buffer;
    std::string line;
    char        c;

    while (file.get(c)) {
        if (is_char(c)) { // printable character
            buffer += c;
        } else if (!buffer.empty()) { // end of a string
            if (buffer.size() >= MIN_STRING_LEN && is_printable_string(buffer)) {
                QString* nstr = new QString(buffer.c_str());
                result->push_back(*nstr);
            }
            buffer.clear();
        }
    }

    if (!buffer.empty()) {
        if (buffer.size() >= MIN_STRING_LEN && is_printable_string(buffer)) {
            QString* nstr = new QString(buffer.c_str());
            result->push_back(*nstr);
        }
        buffer.clear();
    }
    file.close();
    return result;
}

qint64 Analyze::getDuration() const {
    return duration;
}

void Analyze::setDuration(qint64 newDuration) {
    duration = newDuration;
}
