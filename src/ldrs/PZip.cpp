#include "PZip.h"

void PZip::setRootPath(const QString &path) {
    m_rootPath = path;
}

QString PZip::rootPath() const {
    return m_rootPath;
}

bool PZip::open(const QString &filePath, int mode) {
    QuaZip zip(filePath);
    if (!zip.open(mode)) {
        qDebug() << "Failed to open zip file:" << filePath;
        return false;
    }
    return true;
}

bool PZip::close() {
    QuaZip zip(m_rootPath);
    if (!zip.close()) {
        qDebug() << "Failed to close zip file:" << m_rootPath;
        return false;
    }
    return true;
}

PFileData PZip::read(const QString &filePath) {
    QuaZip zip(m_rootPath);
    if (!zip.open(QuaZip::mdUnzip)) {
        qDebug() << "Failed to open zip file:" << m_rootPath;
        return QByteArray();
    }

    QuaZipFile file(&zip);
    if (!file.open(QIODevice::ReadOnly, QuaZipNewInfo(filePath))) {
        qDebug() << "Failed to open file in zip:" << filePath;
        return QByteArray();
    }

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
    QuaZip zip(m_rootPath);
    if (!zip.open(QuaZip::mdCreate)) {
        qDebug() << "Failed to open zip file for writing:" << m_rootPath;
        return false;
    }

    QuaZipFile file(&zip);
    if (!file.open(QIODevice::WriteOnly, QuaZipNewInfo(filePath))) {
        qDebug() << "Failed to open file in zip for writing:" << filePath;
        return false;
    }

    file.write(data.data);
    file.close();
    zip.close();
    return true;
}

bool PZip::remove(const QString &filePath) {
    QuaZip zip(m_rootPath);
    if (!zip.open(QuaZip::mdUnzip)) {
        qDebug() << "Failed to open zip file for removing:" << m_rootPath;
        return false;
    }

    if (!zip.remove(filePath)) {
        qDebug() << "Failed to remove file from zip:" << filePath;
        return false;
    }

    zip.close();
    return true;
}

bool PZip::exists(const QString &filePath) {
    QuaZip zip(m_rootPath);
    if (!zip.open(QuaZip::mdUnzip)) {
        qDebug() << "Failed to open zip file for checking existence:" << m_rootPath;
        return false;
    }

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
    QuaZip zip(m_rootPath);
    if (!zip.open(QuaZip::mdUnzip)) {
        qDebug() << "Failed to open zip file for copying:" << m_rootPath;
        return false;
    }

    QuaZipFile file(&zip);
    if (!file.open(QIODevice::ReadOnly, QuaZipNewInfo(filePath))) {
        qDebug() << "Failed to open file in zip for copying:" << filePath;
        return false;
    }

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
    QuaZip zip(m_rootPath);
    if (!zip.open(QuaZip::mdUnzip)) {
        qDebug() << "Failed to open zip file for replacing:" << m_rootPath;
        return false;
    }

    QuaZipFile file(&zip);
    if (!file.open(QIODevice::WriteOnly, QuaZipNewInfo(filePath))) {
        qDebug() << "Failed to open file in zip for replacing:" << filePath;
        return false;
    }

    if (!write(filePath, data)) {
        qDebug() << "Failed to write data to file in zip:" << filePath;
        return false;
    }
    file.close();
    zip.close();
    return true;
}

bool PZip::makeDir(const QString &dirPath) {
    QuaZip zip(m_rootPath);
    if (!zip.open(QuaZip::mdCreate)) {
        qDebug() << "Failed to open zip file for creating directory:" << m_rootPath;
        return false;
    }

    if (!zip.mkdir(dirPath)) {
        qDebug() << "Failed to create directory in zip:" << dirPath;
        return false;
    }

    zip.close();
    return true;
}

bool PZip::dirExists(const QString &dirPath) {
    QuaZip zip(m_rootPath);
    if (!zip.open(QuaZip::mdUnzip)) {
        qDebug() << "Failed to open zip file for checking directory existence:" << m_rootPath;
        return false;
    }

    bool exists = zip.findFile(dirPath);
    zip.close();
    return exists;
}

bool PZip::removeDir(const QString &dirPath) {
    QuaZip zip(m_rootPath);
    if (!zip.open(QuaZip::mdUnzip)) {
        qDebug() << "Failed to open zip file for removing directory:" << m_rootPath;
        return false;
    }

    if (!zip.remove(dirPath)) {
        qDebug() << "Failed to remove directory from zip:" << dirPath;
        return false;
    }

    zip.close();
    return true;
}

bool PZip::listFiles(const QString &dirPath) {
    QuaZip zip(m_rootPath);
    if (!zip.open(QuaZip::mdUnzip)) {
        qDebug() << "Failed to open zip file for listing files:" << m_rootPath;
        return false;
    }

    QuaZipFile file(&zip);
    if (!file.open(QIODevice::ReadOnly, QuaZipNewInfo(dirPath))) {
        qDebug() << "Failed to open directory in zip for listing files:" << dirPath;
        return false;
    }

    // TODO: Implement file listing logic here
    // For now, just close the file and zip

    file.close();
    zip.close();
    return true;
}