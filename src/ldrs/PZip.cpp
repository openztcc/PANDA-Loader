#include "PZip.h"

PZip::PZip(const QString &filePath) : m_rootPath("") {
    if (filePath.isEmpty() || filePath == "") {
        qDebug() << "No file path provided for PZip: " << filePath;
        return;
    }

    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists()) {
        qDebug() << "Zip file does not exist:" << filePath;
    } else if (!fileInfo.isFile()) {
        qDebug() << "Zip file is not a valid file:" << filePath;
    } else {
        m_rootPath = fileInfo.absoluteFilePath();
        qDebug() << "Zip file path set to:" << m_rootPath;
    }
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
        qDebug() << "Failed to open zip file (openZip):" << filePath;
        return QSharedPointer<QuaZip>::create();
    }
    return zip;
}

QSharedPointer<QuaZipFile> PZip::openZipFile(QSharedPointer<QuaZip> &zip, const QString &relPath, QIODevice::OpenMode mode) {
    if (!zip->setCurrentFile(relPath)) {
        qWarning() << "Failed to set current file in zip:" << relPath;
        return nullptr;
    }

    auto file = QSharedPointer<QuaZipFile>::create(zip.data());

    if (!file->open(mode)) {
        qWarning() << "Failed to open current file in zip:" << relPath
                   << "Error:" << file->getZipError();
        return nullptr;
    }

    return file;
}

// Read a file from the zip archive
// usage: PFileData fileData = zip.read("path/to/file.txt");
PFileData PZip::read(const QString &relFilePath) {
    QSharedPointer<QuaZip> zip = openZip(m_rootPath, QuaZip::mdUnzip);

    QSharedPointer<QuaZipFile> file = openZipFile(zip, relFilePath, QIODevice::ReadOnly);
    if (!file) {
        qDebug() << "Failed to open file in zip:" << relFilePath;
        return PFileData();
    }

    PFileData fileData;
    fileData.filename = relFilePath.section('/', -1, -1);
    fileData.ext = relFilePath.section('.', -1, -1);
    fileData.path = relFilePath.section('/', 0, -2) + '/';

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

// Write a file to the zip archive given a PFileData object
// 
bool PZip::write(const PFileData &data) {
    QSharedPointer<QuaZip> zip = openZip(m_rootPath, QuaZip::mdAdd);

    QuaZipFile file(zip.data());
    // path to write to
    QuaZipNewInfo info(data.path + data.filename);
    qDebug() << "Setting relative path:" << data.path + data.filename;
    if (!file.open(QIODevice::WriteOnly, info)) {
        qDebug() << "Failed to open file for writing in zip:" << data.path + data.filename;
        return false;
    }
    
    file.write(data.data);
    file.close();
    zip->close();
    return true;
}

bool PZip::remove(const QStringList &itemsToRemove) {
    QuaZip openedZip = openZip(m_rootPath, QuaZip::mdUnzip);

    // Create a temporary file that will be the end output
    QString temp(m_rooTPath + ".tmp");
    QuaZip outZip = openZip(temp, QuaZip::mdCreate);

    QuaZipFile fileToCopy(&openedZip);

    // copy all files except the ones to remove
    for (bool next = openedZip.goToFirstFile(); next; next = openedZip.goToNextFile()) {
        // start by getting the current file name (could be a directory so will need to check)
        QString currentFileName = openedZip.getCurrentFileName();

        // check if file is in our list of files to remove. if yes, skip.
        if (itemsToRemove.contains(fileName)) {
            qDebug() << "Removing file from zip:" << fileName;
            continue;
        }

        // if not, copy the file to the new zip file
        if (!fileToCopy.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to open file for reading in zip:" << fileName;
            continue;
        }

        // create a new file in the new zip with the same name as the current file
        QuaZipNewInfo newFileInfo(fileName);
        QuaZipFile newFile(&outZip);
        if (!newFile.open(QIODevice::WriteOnly, newFileInfo)) {
            qDebug() << "Failed to open file for writing in zip:" << fileName;
            continue;
        }

        // read the data from the current file and write it to the new file
        QByteArray data = fileToCopy.readAll();
        newFile.write(data);
        newFile.close();
        fileToCopy.close();
    }

    // close the zip files
    openedZip.close();
    outZip.close();

    // remove the original zip file
    if (!QFile::remove(m_rootPath)) {
        qDebug() << "Failed to remove original zip file:" << m_rootPath;
        return false;
    }

    // rename the temporary zip file to the original zip file name
    if (!QFile::rename(temp, m_rootPath)) {
        qDebug() << "Failed to rename temporary zip file:" << temp << "to" << m_rootPath;
        return false;
    }
    qDebug() << "Successfully removed files from zip:" << m_rootPath;
}


// Remove a single file from the zip archive
// usage: bool success = zip.remove("path/to/file.txt");
bool PZip::remove(const QString &itemToRemove) {
    QStringList itemsToRemove;
    itemsToRemove.append(itemToRemove);
    return remove(itemsToRemove);
}

bool PZip::exists(const QString &relFilePath) {
    if (read(relFilePath).data.isEmpty()) {
        qDebug() << "File does not exist in zip:" << relFilePath;
        return false;
    }
    qDebug() << "File exists in zip:" << relFilePath;
    return true;
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

    QSharedPointer<QuaZipFile> file = openZipFile(zip, "", QIODevice::ReadOnly);

    PFileData fileData;
    QByteArray data = file->readAll();
    fileData.data = data;
    fileData.filename = file->getFileName();
    fileData.ext = file->getFileName().section('.', -1, -1);
    fileData.path = file->getFileName().section('/', 0, -2);
    file->close();
    zip->close();

    return write(fileData);
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

    QSharedPointer<QuaZipFile> file = openZipFile(zip, "", QIODevice::ReadOnly);

    if (!write(data)) {
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

    QSharedPointer<QuaZipFile> file = openZipFile(zip, "", QIODevice::ReadOnly);

    // TODO: Implement file listing logic here
    // For now, just close the file and zip

    file->close();
    zip->close();
    return true;
}
