#include "PFileSystem.h"

void PFileSystem::setRootPath(const QString &path) {
    m_rootPath = path;
}

QString PFileSystem::rootPath() const {
    return m_rootPath;
}

PFileData PFileSystem::read(const QString &filePath) {
    QFile file(m_rootPath + "/" + filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file for reading:" << filePath;
        return QByteArray();
    }
    QByteArray data = file.readAll();
    file.close();
    return data;
}

bool PFileSystem::write(const QString &filePath, const PFileData &data) {
    QFile file(m_rootPath + "/" + filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open file for writing:" << filePath;
        return false;
    }
    file.write(data);
    file.close();
    return true;
}

bool PFileSystem::remove(const QString &filePath) {
    QFile file(m_rootPath + "/" + filePath);
    if (!file.remove()) {
        qDebug() << "Failed to remove file:" << filePath;
        return false;
    }
    return true;
}

bool PFileSystem::exists(const QString &filePath) {
    QFile file(m_rootPath + "/" + filePath);
    return file.exists();
}

bool PFileSystem::move(const QString &filePath, const QString &newLocation) {
    if (copy(filePath, newLocation)) {
        return remove(filePath);
    } else {
        qDebug() << "Failed to move file:" << filePath << "to" << newLocation;
        return false;
    }
}

bool PFileSystem::copy(const QString &filePath, const QString &newLocation) {
    QFile file(m_rootPath + "/" + filePath);
    if (!file.copy(newLocation)) {
        qDebug() << "Failed to copy file:" << filePath << "to" << newLocation;
        return false;
    }
    return true;
}

bool PFileSystem::rename(const QString &filePath, const QString &newFileName) {
    if (!copy(filePath, newFileName)) {
        qDebug() << "Failed to rename file:" << filePath << "to" << newFileName;
        return false;
    }
}

bool PFileSystem::replace(const QString &filePath, const PFileData &data) {
    QFile file(m_rootPath + "/" + filePath);
    if (!remove(filePath)) {
        qDebug() << "Failed to remove file:" << filePath;
        return false;
    }

    if (!write(filePath, data)) {
        qDebug() << "Failed to write data to file:" << filePath;
        return false;
    }
    return true;
}

bool PFileSystem::makeDir(const QString &dirPath) {
    QDir dir(m_rootPath + "/" + dirPath);
    if (!dirExists()) {
        if (!dir.mkpath(dirPath)) {
            qDebug() << "Failed to create directory:" << dirPath;
            return false;
        }
    } else {
        qDebug() << "Directory already exists:" << dirPath;
    }
    return true;
}

bool PFileSystem::dirExists(const QString &dirPath) {
    QDir dir(m_rootPath + "/" + dirPath);
    return dir.exists();
}

bool PFileSystem::removeDir(const QString &dirPath) {
    QDir dir(m_rootPath + "/" + dirPath);
    if (!dir.removeRecursively()) {
        qDebug() << "Failed to remove directory:" << dirPath;
        return false;
    }
    return true;
}

bool PFileSystem::listFiles(const QString &dirPath) {
    QDir dir(m_rootPath + "/" + dirPath);
    if (!dirExists()) {
        qDebug() << "Directory does not exist:" << dirPath;
        return false;
    }
    QStringList files = dir.entryList(QDir::Files);
    for (const QString &file : files) {
        qDebug() << "File:" << file;
    }
    return true;
}
