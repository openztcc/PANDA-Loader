#include "PFile.h"

PFile::PFile(QObject *parent, const QString &filePath, FileType type)
    : QObject(parent), m_rootPath(""), m_file(nullptr) {
    // Initialize the virtual filesystem object
    m_file = createFilesystem(filePath, type);

}

// Create a virtual filesystem object
std::unique_ptr<IVirtualFilesystem> PFile::createFilesystem(const QString &path, FileType type) const
{
    if (type == FileType::Zip) {
        qDebug() << "Creating PZip filesystem for path:" << path;
        return std::make_unique<PZip>(path);
    } else {
        qDebug() << "Creating PFileSystem filesystem for path:" << path;
        return std::make_unique<PFileSystem>();
    }
}

PFileData PFile::read(const QString &relFilePath) {
    // Read the file from the virtual filesystem
    return m_file->read(relFilePath);
}

QList<PFileData> PFile::readAll(const QStringList& validFolders, const QStringList &validExts) {
    return m_file->readAll(validFolders, validExts);
}

bool PFile::write(const PFileData &data) {
    // Write the file to the virtual filesystem
    return m_file->write(data);
}

bool PFile::remove(const QStringList &filesToRemove) {
    // Remove the file from the virtual filesystem
    return m_file->remove(filesToRemove);
}

bool PFile::remove(const QString &fileToRemove) {
    // Remove the file from the virtual filesystem
    return m_file->remove({fileToRemove});
}

bool PFile::exists(const QString &filePath) {
    // Check if the file exists in the virtual filesystem
    return m_file->exists(filePath);
}

bool PFile::move(const QString &filePath, const QString &newLocation) {
    // Move the file in the virtual filesystem
    return m_file->move(filePath, newLocation);
}

bool PFile::copy(const QString &filePath, const QString &newLocation) {
    // Copy the file in the virtual filesystem
    return m_file->copy(filePath, newLocation);
}

bool PFile::rename(const QString &filePath, const QString &newFileName) {
    // Rename the file in the virtual filesystem
    return m_file->rename(filePath, newFileName);
}

void PFile::setRootPath(const QString &path) {
    m_rootPath = path;
}

QString PFile::rootPath() const {
    return m_rootPath;
}
