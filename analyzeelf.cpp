#include "analyzeelf.h"

#include <elf.h>
#include <fcntl.h>
#include <libelf.h>

#include <QBitArray>
#include <QDataStream>
#include <QDebug>
#include <QFile>
#include <QMap>
#include <QMessageBox>
#include <cstring>
#include <iostream>
#include <map>

#include "binarysection.h"

AnalyzeElf::AnalyzeElf(QString filepath) {
    this->filepath = filepath;
}

AnalyzeElf::~AnalyzeElf() {}
QVector<BinarySection>* AnalyzeElf::GetSections() {
    std::cout << "Debut getsection" << std::endl;

    sections = new QVector<BinarySection>();
    QFile file(this->filepath);

    if (!file.open(QIODevice::ReadOnly)) {
        // handle error opening file
        return sections;
    }

    // read ELF header
    Elf64_Ehdr elfHeader;
    if (file.read((char*) &elfHeader, sizeof(Elf64_Ehdr)) != sizeof(Elf64_Ehdr)) {
        // handle error reading ELF header
        file.close();
        return sections;
    }

    // read section header table
    Elf64_Shdr sectionHeaderTable[elfHeader.e_shnum];
    if (file.seek(elfHeader.e_shoff) &&
        file.read((char*) sectionHeaderTable, sizeof(Elf64_Shdr) * elfHeader.e_shnum) !=
            sizeof(Elf64_Shdr) * elfHeader.e_shnum) {
        // handle error reading section header table
        file.close();
        return sections;
    }

    // iterate over section headers
    for (int i = 0; i < elfHeader.e_shnum; i++) {
        QString name;
        if (elfHeader.e_shstrndx != SHN_UNDEF) {
            // read section name from section header string table
            Elf64_Shdr strHeader = sectionHeaderTable[elfHeader.e_shstrndx];
            char*      strTable  = new char[strHeader.sh_size];
            if (file.seek(strHeader.sh_offset) &&
                file.read(strTable, strHeader.sh_size) == strHeader.sh_size) {
                name = QString::fromLatin1(strTable + sectionHeaderTable[i].sh_name);
            }
            delete[] strTable;
        }

        // create BinarySection object and add it to the vector
        auto perms = new QBitArray();
        perms->resize(3);
        perms->setBit(0, (sectionHeaderTable[i].sh_flags & SHF_ALLOC) > 1);
        perms->setBit(1, (sectionHeaderTable[i].sh_flags & SHF_WRITE) > 1);
        perms->setBit(2, (sectionHeaderTable[i].sh_flags & SHF_EXECINSTR) > 1);

        sections->append(BinarySection(name, sectionHeaderTable[i].sh_offset,
                                       sectionHeaderTable[i].sh_size, *perms, 0.0));
    }
    std::cout << "fin getsection" << std::endl;

    file.close();
    return sections;
}

QMap<QString, QString>* AnalyzeElf::ParseHeader() {
    // Ouvre le fichier ELF en lecture
    QFile file(this->filepath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Impossible d'ouvrir le fichier ELF en lecture.";
        return nullptr;
    }

    // Lit les 16 premiers octets pour obtenir la signature ELF
    char elf_signature[16];
    if (file.read(elf_signature, 16) != 16) {
        qDebug() << "Erreur de lecture de la signature ELF.";
        return nullptr;
    }

    // Vérifie que la signature est valide
    if (memcmp(elf_signature, ELFMAG, SELFMAG) != 0) {
        qDebug() << "Signature ELF invalide.";
        return nullptr;
    }

    // Lit le reste de l'en-tête ELF
    Elf32_Ehdr header;
    if (file.read(reinterpret_cast<char*>(&header), sizeof(header)) != sizeof(header)) {
        qDebug() << "Erreur de lecture de l'en-tête ELF.";
        return nullptr;
    }

    // Convertit les champs de l'en-tête en chaînes de caractères
    QMap<QString, QString>* header_map = new QMap<QString, QString>;
    header_map->insert("Type", QString::number(header.e_type));
    header_map->insert("Machine", QString::number(header.e_machine));
    header_map->insert("Version", QString::number(header.e_version));
    header_map->insert("Entry", QString::number(header.e_entry, 16));
    header_map->insert("Phoff", QString::number(header.e_phoff));
    header_map->insert("Shoff", QString::number(header.e_shoff));
    header_map->insert("Flags", QString::number(header.e_flags));
    header_map->insert("Ehsize", QString::number(header.e_ehsize));
    header_map->insert("Phentsize", QString::number(header.e_phentsize));
    header_map->insert("Phnum", QString::number(header.e_phnum));
    header_map->insert("Shentsize", QString::number(header.e_shentsize));
    header_map->insert("Shnum", QString::number(header.e_shnum));
    header_map->insert("Shstrndx", QString::number(header.e_shstrndx));

    return header_map;
}

QVector<QVector<QString>*>* AnalyzeElf::GetImports() {}
