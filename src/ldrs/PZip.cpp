#include "PZip.h"

void PZip::setRootPath(const QString &path) {
    m_rootPath = path;
}

QString PZip::rootPath() const {
    return m_rootPath;
}

QuaZip PZip::openZip(const QString &filePath, int mode) {
    QuaZip zip(filePath);
    if (!zip.open(mode)) {
        qDebug() << "Failed to open zip file:" << filePath;
        return false;
    }
    return true;
}

QuaZipFile PZip::openZipFile(QuaZip &zip, int mode) {
    QuaZipFile file(&zip);
    if (!file.open(mode)) {
        qDebug() << "Failed to open zip file:" << zip.getZipName();
        return false;
    }
    return true;
}

PFileData PZip::read(const QString &filePath) {
    QuaZip zip = open(m_rootPath, QuaZip::mdUnzip);

    QuaZipFile file = openZipFile(zip, QuaZip::mdUnzip);

    PFileData fileData;
    fileData.filename = filePath.section('/', -1, -1);
    fileData.ext = QFileInfo(filePath).suffix();
    fileData.path = filePath.section('/', 0, -2);

    QByteArray data = file.readAll();
    file.close();
    zip.close();

    fileData.data = data;

    // print file data for debugging
    qDebug() << "File data size:" << fileData.data.size();
    qDebug() << "File name:" << fileData.filename;
    qDebug() << "File ext:" << fileData.ext;
    qDebug() << "File path:" << fileData.path;

    return fileData;
}

bool PZip::write(const QString &filePath, const PFileData &data) {
    QuaZip zip = open(m_rootPath, QuaZip::mdCreate);

    QuaZipFile file = openZipFile(zip, QuaZip::mdCreate);

    file.write(data.data);
    file.close();
    zip.close();
    return true;
}

bool PZip::remove(const QString &filePath) {
    QuaZip zip = openZip(m_rootPath, QuaZip::mdUnzip);

    if (!zip.remove(filePath)) {
        qDebug() << "Failed to remove file from zip:" << filePath;
        return false;
    }

    zip.close();
    return true;
}

bool PZip::exists(const QString &filePath) {
    QuaZip zip = openZip(m_rootPath, QuaZip::mdUnzip);

    bool exists = zip.findFile(filePath);
    zip.close();
    return exists;
}

bool PZip::move(const QString &filePath, const QString &newLocation) {
    if (!copy(filePath, newLocation)) {
        qDebug() << "Failed to move file:" << filePath << "to" << newLocation;
        return false;
    }
    return remove(filePath);
}

bool PZip::copy(const QString &filePath, const QString &newLocation) {
    QuaZip zip = openZip(m_rootPath, QuaZip::mdUnzip);

    QuaZipFile file = openZipFile(zip, QuaZip::mdUnzip);

    QByteArray data = file.readAll();
    file.close();
    zip.close();

    return write(newLocation, data);
}

bool PZip::rename(const QString &filePath, const QString &newFileName) {
    if (!copy(filePath, newFileName)) {
        qDebug() << "Failed to rename file:" << filePath << "to" << newFileName;
        return false;
    }
    return remove(filePath);
}

bool PZip::replace(const QString &filePath, const PFileData &data) {
    QuaZip zip = openZip(m_rootPath, QuaZip::mdUnzip);

    QuaZipFile file = openZipFile(zip, QuaZip::mdUnzip);

    if (!write(filePath, data)) {
        qDebug() << "Failed to write data to file in zip:" << filePath;
        return false;
    }
    file.close();
    zip.close();
    return true;
}

bool PZip::makeDir(const QString &dirPath) {
    QuaZip zip = openZip(m_rootPath, QuaZip::mdCreate);

    if (!zip.mkdir(dirPath)) {
        qDebug() << "Failed to create directory in zip:" << dirPath;
        return false;
    }

    zip.close();
    return true;
}

bool PZip::dirExists(const QString &dirPath) {
    QuaZip zip = openZip(m_rootPath, QuaZip::mdUnzip);

    bool exists = zip.findFile(dirPath);
    zip.close();
    return exists;
}

bool PZip::removeDir(const QString &dirPath) {
    QuaZip zip = openZip(m_rootPath, QuaZip::mdUnzip);

    if (!zip.remove(dirPath)) {
        qDebug() << "Failed to remove directory from zip:" << dirPath;
        return false;
    }

    zip.close();
    return true;
}

bool PZip::listFiles(const QString &dirPath) {
    QuaZip zip = openZip(m_rootPath, QuaZip::mdUnzip);

    QuaZipFile file = openZipFile(zip, QuaZip::mdUnzip);

    // TODO: Implement file listing logic here
    // For now, just close the file and zip

    file.close();
    zip.close();
    return true;
}