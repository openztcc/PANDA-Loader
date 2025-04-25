#include "PFileSystem.h"

void PFileSystem::setRootPath(const QString &path) {
    m_rootPath = path;
}

QString PFileSystem::rootPath() const {
    return m_rootPath;
}

QSharedPointer<QFile> PFileSystem::openFile(const QString &filePath, QIODevice::OpenMode mode) {
    auto file = QSharedPointer<QFile>::create(m_rootPath + "/" + filePath);
    if (!file->open(mode)) {
        qDebug() << "Failed to open file:" << filePath;
    }
    return file;
}

PFileData PFileSystem::read(const QString &filePath) {
    QSharedPointer<QFile> file = openFile(m_rootPath + "/" + filePath, QIODevice::ReadOnly);
    
    QFileInfo fileInfo(filePath);
    PFileData fileData;
    fileData.filename = fileInfo.fileName();
    fileData.ext = fileInfo.suffix();
    fileData.path = fileInfo.absolutePath();
    QByteArray data = file->readAll();
    fileData.data = data;

    file->close();

    return fileData;
}

bool PFileSystem::write(const PFileData &data) {
    QString filePath = data.path + "/" + data.filename;
    QSharedPointer<QFile> file = openFile(filePath, QIODevice::WriteOnly);
    file->write(data.data);
    file->close();
    return true;
}

bool PFileSystem::remove(const QStringList &itemsToRemove) {
    for (const QString &item : itemsToRemove) {
        QString filePath = m_rootPath + "/" + item;
        if (!QFile::remove(filePath)) {
            qDebug() << "Failed to remove file:" << filePath;
            return false;
        }
    }
    return true;
}

bool PFileSystem::remove(const QString &itemToRemove) {
    return remove({itemToRemove});
}

bool PFileSystem::exists(const QString &filePath) {
    QSharedPointer<QFile> file = openFile(m_rootPath + "/" + filePath, QIODevice::ReadOnly);
    return file->exists();
}

bool PFileSystem::move(const QString &filePath, const QString &newLocation) {
    QString localPath = m_rootPath + "/" + filePath;
    QString newPath = m_rootPath + "/" + newLocation;
    if (!QFile::rename(localPath, newPath)) {
        qDebug() << "Failed to move file:" << localPath << "to" << newPath;
        return false;
    }
    return true;
}

bool PFileSystem::copy(const QString &filePath, const QString &newLocation) {
    QSharedPointer<QFile> file = openFile(m_rootPath + "/" + filePath, QIODevice::ReadOnly);
    if (!file->copy(newLocation)) {
        qDebug() << "Failed to copy file:" << filePath << "to" << newLocation;
        return false;
    }
    return true;
}

bool PFileSystem::rename(const QString &filePath, const QString &newFileName) {
    QFileInfo fileInfo(m_rootPath + "/" + filePath);
    QString newFilePath = fileInfo.absolutePath() + "/" + newFileName;
    if (!QFile::rename(fileInfo.absoluteFilePath(), newFilePath)) {
        qDebug() << "Failed to rename file:" << filePath << "to" << newFileName;
        return false;
    }
    return true;
}