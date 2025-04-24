#include "PZip.h"

PZip::PZip() : m_rootPath("") {
    QFileInfo fileInfo(m_rootPath);
    if (!fileInfo.exists()) {
        qDebug() << "File path does not exist:" << m_rootPath;
    } else if (!fileInfo.isDir()) {
        qDebug() << "File path is not a directory:" << m_rootPath;
    } else {
        qDebug() << "File path is valid:" << m_rootPath;
    }
    m_rootPath = fileInfo.absoluteFilePath();
}

void PZip::setRootPath(const QString &path) {
    m_rootPath = path;
}

QString PZip::rootPath() const {
    return m_rootPath;
}

QSharedPointer<QuaZip> PZip::openZip(const QString &filePath, QuaZip::Mode mode) {
    auto zip = QSharedPointer<QuaZip>::create(filePath);
    if (!zip->open(mode)) {
        qDebug() << "Failed to open zip file:" << filePath;
        return QSharedPointer<QuaZip>::create();
    }
    return zip;
}

QSharedPointer<QuaZipFile> PZip::openZipFile(QSharedPointer<QuaZip> &zip, QIODevice::OpenMode mode) {
    auto file = QSharedPointer<QuaZipFile>::create(zip.data());
    if (!file->open(mode)) {
        qDebug() << "Failed to open zip file:" << zip->getZipName();
        return QSharedPointer<QuaZipFile>::create();
    }
    return file;
}

PFileData PZip::read(const QString &filePath) {
    QSharedPointer<QuaZip> zip = openZip(m_rootPath, QuaZip::mdUnzip);

    QSharedPointer<QuaZipFile> file = openZipFile(zip, QIODevice::ReadOnly);

    PFileData fileData;
    fileData.filename = filePath.section('/', -1, -1);
    fileData.ext = QFileInfo(filePath).suffix();
    fileData.path = filePath.section('/', 0, -2);

    QByteArray data = file->readAll();
    file->close();
    zip->close();

    fileData.data = data;

    // print file data for debugging
    qDebug() << "File data size:" << fileData.data.size();
    qDebug() << "File name:" << fileData.filename;
    qDebug() << "File ext:" << fileData.ext;
    qDebug() << "File path:" << fileData.path;

    return fileData;
}

bool PZip::write(const QString &filePath, const PFileData &data) {
    QSharedPointer<QuaZip> zip = openZip(m_rootPath, QuaZip::mdCreate);

    QSharedPointer<QuaZipFile> file = openZipFile(zip, QIODevice::WriteOnly);

    file->write(data.data);
    file->close();
    zip->close();
    return true;
}

bool PZip::remove(const QString &filePath) {
    QSharedPointer<QuaZip> zip = openZip(m_rootPath, QuaZip::mdUnzip);

    // if (!zip->remove(filePath)) {
    //     qDebug() << "Failed to remove file from zip:" << filePath;
    //     return false;
    // }

    zip->close();
    return true;
}

bool PZip::exists(const QString &filePath) {
    QSharedPointer<QuaZip> zip = openZip(m_rootPath, QuaZip::mdUnzip);

    bool exists = true;//zip->findFile(filePath);
    zip->close();
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
    QSharedPointer<QuaZip> zip = openZip(m_rootPath, QuaZip::mdUnzip);

    QSharedPointer<QuaZipFile> file = openZipFile(zip, QIODevice::ReadOnly);

    PFileData fileData;
    QByteArray data = file->readAll();
    fileData.data = data;
    fileData.filename = file->getFileName();
    fileData.ext = file->getFileName().section('.', -1, -1);
    fileData.path = file->getFileName().section('/', 0, -2);
    file->close();
    zip->close();

    return write(newLocation, fileData);
}

bool PZip::rename(const QString &filePath, const QString &newFileName) {
    if (!copy(filePath, newFileName)) {
        qDebug() << "Failed to rename file:" << filePath << "to" << newFileName;
        return false;
    }
    return remove(filePath);
}

bool PZip::replace(const QString &filePath, const PFileData &data) {
    QSharedPointer<QuaZip> zip = openZip(m_rootPath, QuaZip::mdUnzip);

    QSharedPointer<QuaZipFile> file = openZipFile(zip, QIODevice::ReadOnly);

    if (!write(filePath, data)) {
        qDebug() << "Failed to write data to file in zip:" << filePath;
        return false;
    }
    file->close();
    zip->close();
    return true;
}

bool PZip::makeDir(const QString &dirPath) {
    QSharedPointer<QuaZip> zip = openZip(m_rootPath, QuaZip::mdCreate);

    // if (!zip->mkdir(dirPath)) {
    //     qDebug() << "Failed to create directory in zip:" << dirPath;
    //     return false;
    // }

    zip->close();
    return true;
}

bool PZip::dirExists(const QString &dirPath) {
    QSharedPointer<QuaZip> zip = openZip(m_rootPath, QuaZip::mdUnzip);

    bool exists = true;//zip->findFile(dirPath);
    zip->close();
    return exists;
}

bool PZip::removeDir(const QString &dirPath) {
    QSharedPointer<QuaZip> zip = openZip(m_rootPath, QuaZip::mdUnzip);

    // if (!zip->remove(dirPath)) {
    //     qDebug() << "Failed to remove directory from zip:" << dirPath;
    //     return false;
    // }

    zip->close();
    return true;
}

bool PZip::listFiles(const QString &dirPath) {
    QSharedPointer<QuaZip> zip = openZip(m_rootPath, QuaZip::mdUnzip);

    QSharedPointer<QuaZipFile> file = openZipFile(zip, QIODevice::ReadOnly);

    // TODO: Implement file listing logic here
    // For now, just close the file and zip

    file->close();
    zip->close();
    return true;
}
