#ifndef PFILEDATA_H
#define PFILEDATA_H

#include <QDir>

struct PFileData {
    QByteArray data;
    QString filename;
    QString ext;
    QString path;
};

#endif // PFILEDATA_H