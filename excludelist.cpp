#include "excludelist.h"

#include <zlib.h>

#include <QFile>
#include <QMessageBox>
#include <QResource>
#include <QString>
#include <QVector>
#include <iostream>

ExcludeList::ExcludeList() {}

ExcludeList::ExcludeList(QString name) {
    this->name = name;
    this->list = new QVector<QString>();
}

ExcludeList::ExcludeList(QString list_name, bool activated) {
    // Initialize resources
    Q_INIT_RESOURCE(resources);

    // Load the resource file
    QString resource_path = ":/list_pack/resources/" + list_name + ".gz";
    QFile   res_file(resource_path);
    if (!res_file.open(QIODevice::ReadOnly)) {
        std::cerr << "Error: Could not open resource file: " << resource_path.toStdString()
                  << std::endl;
        return;
    }

    // Uncompress the data
    const int BUFFER_SIZE = 1024;
    char      buffer[BUFFER_SIZE];

    QByteArray uncompressedData;
    z_stream   stream;
    stream.zalloc   = Z_NULL;
    stream.zfree    = Z_NULL;
    stream.opaque   = Z_NULL;
    stream.avail_in = 0;
    stream.next_in  = Z_NULL;
    if (inflateInit2(&stream, 16 + MAX_WBITS) != Z_OK) {
        std::cerr << "Error: Failed to initialize zlib stream" << std::endl;
        return;
    }

    int bytesRead;
    while ((bytesRead = res_file.read(buffer, BUFFER_SIZE)) > 0) {
        stream.avail_in = bytesRead;
        stream.next_in  = reinterpret_cast<Bytef*>(buffer);

        do {
            char out_buffer[BUFFER_SIZE];
            stream.avail_out = BUFFER_SIZE;
            stream.next_out  = reinterpret_cast<Bytef*>(out_buffer);
            int ret          = inflate(&stream, Z_NO_FLUSH);
            if (ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
                std::cerr << "Error: Failed to decompress data" << std::endl;
                inflateEnd(&stream);
                return;
            }
            int have = BUFFER_SIZE - stream.avail_out;
            uncompressedData.append(out_buffer, have);
        } while (stream.avail_out == 0);
    }

    if (bytesRead == -1) {
        std::cerr << "Error: Could not read from resource file: " << resource_path.toStdString()
                  << std::endl;
        inflateEnd(&stream);
        return;
    }

    inflateEnd(&stream);

    // Close the resource file
    res_file.close();

    this->setIs_activated(activated);
    this->setName(list_name);
    QVector<QString>* new_list = new QVector<QString>();
    for (auto li : uncompressedData.split('\n')) {
        new_list->append(QString(li));
    }
    this->setList(new_list);
}

bool ExcludeList::getIs_activated() const {
    return is_activated;
}

void ExcludeList::setIs_activated(bool newIs_activated) {
    is_activated = newIs_activated;
}

QVector<QString>* ExcludeList::getList() const {
    return list;
}

void ExcludeList::setList(QVector<QString>* newList) {
    list = newList;
}

void ExcludeList::add_item(QString item) {
    this->list->append(item);
}

QString ExcludeList::getName() const {
    return name;
}

void ExcludeList::setName(const QString& newName) {
    name = newName;
}
