#include <PFile.h>

// Create a virtual filesystem object
std::unique_ptr<IVirtualFilesystem> PFileSystem::createFilesystem(const QString &path) const
{
    // Get the file extension
    QString ext = QFileInfo(path).suffix().toLower();
    if (ext == "zip") {
        return std::make_unique<PZip>();
    } else {
        return std::make_unique<PFileSystem>();
    }
}

PFileData PFileSystem::read(const QString &filePath) {
    // Read the file from the virtual filesystem
    return m_file->read(filePath);
}

bool PFileSystem::write(const QString &filePath, const PFileData &data) {
    // Write the file to the virtual filesystem
    return m_file->write(filePath, data);
}

bool PFileSystem::remove(const QString &filePath) {
    // Remove the file from the virtual filesystem
    return m_file->remove(filePath);
}

bool PFileSystem::exists(const QString &filePath) {
    // Check if the file exists in the virtual filesystem
    return m_file->exists(filePath);
}

bool PFileSystem::move(const QString &filePath, const QString &newLocation) {
    // Move the file in the virtual filesystem
    return m_file->move(filePath, newLocation);
}

bool PFileSystem::copy(const QString &filePath, const QString &newLocation) {
    // Copy the file in the virtual filesystem
    return m_file->copy(filePath, newLocation);
}

bool PFileSystem::rename(const QString &filePath, const QString &newFileName) {
    // Rename the file in the virtual filesystem
    return m_file->rename(filePath, newFileName);
}

bool PFileSystem::replace(const QString &filePath, const PFileData &data) {
    // Replace the file in the virtual filesystem
    return m_file->replace(filePath, data);
}

bool PFileSystem::makeDir(const QString &dirPath) {
    // Create a directory in the virtual filesystem
    return m_file->makeDir(dirPath);
}

bool PFileSystem::dirExists(const QString &dirPath) {
    // Check if the directory exists in the virtual filesystem
    return m_file->dirExists(dirPath);
}

bool PFileSystem::removeDir(const QString &dirPath) {
    // Remove the directory from the virtual filesystem
    return m_file->removeDir(dirPath);
}

bool PFileSystem::listFiles(const QString &dirPath) {
    // List the files in the directory in the virtual filesystem
    return m_file->listFiles(dirPath);
}

void PFileSystem::setRootPath(const QString &path) {
    m_rootPath = path;
}

QString PFileSystem::rootPath() const {
    return m_rootPath;
}