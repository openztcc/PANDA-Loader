#include "PZip.h"
#include <qregularexpression.h>

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
QSharedPointer<PFileData> PZip::read(const QString &relFilePath) {
    QSharedPointer<QuaZip> zip = openZip(m_rootPath, QuaZip::mdUnzip);

    QSharedPointer<QuaZipFile> file = openZipFile(zip, relFilePath, QIODevice::ReadOnly);
    if (!file) {
        qDebug() << "Failed to open file in zip:" << relFilePath;
        return nullptr;
    }

    QSharedPointer<PFileData> fileData = QSharedPointer<PFileData>::create();
    fileData->filename = relFilePath.section('/', -1, -1);
    fileData->ext = relFilePath.section('.', -1, -1);
    fileData->path = relFilePath.section('/', 0, -2) + '/';

    QByteArray data = file->readAll();
    file->close();
    zip->close();

    fileData->data = data;

    // print file data for debugging
    qDebug() << "File data size:" << fileData->data.size();
    qDebug() << "File name:" << fileData->filename;
    qDebug() << "File ext:" << fileData->ext;
    qDebug() << "File path:" << fileData->path;

    return fileData;
}

// Read all files from the zip archive
// usage: QList<PFileData> files = zip.readAll({"path/to/dir"}, {"txt", "png"});
// note: empty dirs will read all directories and empty exts will read all files,
//       dir with value "/" will read all files in the root directory
//       dir with value "dir/" will read all files in the dir directory
//       ext with value "txt" will read all files with the txt extension
//       ext with value "" will read all files with no extension
QVector<QSharedPointer<PFileData>> PZip::readAll(const QStringList &validDirs, const QStringList &validExts) {
    QVector<QSharedPointer<PFileData>> filesFound;
    QSharedPointer<QuaZip> zip = openZip(m_rootPath, QuaZip::mdUnzip);
    QStringList fileList = zip->getFileNameList();
    QStringList validFiles;

    QSet<int> foundIndices; // to avoid duplicates
    for (const QString &dir : validDirs) {
        for (const QString &path : fileList) {
            // validate paths
            bool validPath = false;

            if (validDirs.isEmpty()) { // fine, we can read all files
                validPath = true;
            } else if (dir == "/") { // root path
                validPath = !path.contains("/");
            } else { // just checks if in validDirs
                validPath = path.startsWith(dir.endsWith("/") ? dir : dir + "/");
            }

            if (!validPath) {
                continue;
            }

            // validate extensions using regex
            bool validExt = false;
            QRegularExpression re("\\.([^\\.]+)$");
            QRegularExpressionMatch match = re.match(path);
            QString ext; 
            if (match.hasMatch()) { // if match get ext *.ext
                ext = match.captured(1);
            } else { // no match, no ext
                ext = "";
            }

            if (validExts.isEmpty()) { // look for all files
                validExt = true;
            } else {
                validExt = validExts.contains(ext);
            }

            if (!validExt) {
                continue;
            }

            // make  sure we avoid duplicates
            int index = fileList.indexOf(path);
            if (!foundIndices.contains(index)) {
                foundIndices.insert(index);
                validFiles.append(path);
            }
        }
    }

    if (validFiles.isEmpty()) {
        qDebug() << "No valid files found in zip:" << m_rootPath;
        return filesFound;
    } else {
        qDebug() << "Print valid files found:" << validFiles.size();
        for (const QString &validFile : validFiles) {
            qDebug() << "Valid file found:" << validFile;
        }
    }

    // read the valid files
    for (const QString &validFile : validFiles) {
        QSharedPointer<PFileData> file = read(validFile);
        if (!file->data.isEmpty()) {
            filesFound.append(file);
        } else {
            qDebug() << "Failed to read file in zip:" << file->filename;
        }
    }
    zip->close();
    return filesFound;
}

// Write a file to the zip archive given a PFileData object
//
bool PZip::write(const QSharedPointer<PFileData> &data) {
    QSharedPointer<QuaZip> zip = openZip(m_rootPath, QuaZip::mdAdd);

    // path to write to
    QString relPath = data->path + data->filename;

    // check if the file already exists in the zip
    if (exists(relPath)) {
        // if it exists, remove it first

        // first close the zip file otherwise it will not be able to remove the file
        zip->close();

        // then remove the file from the zip
        if (!remove(relPath)) {
            return false;
        }

        // reopen the zip file for writing
        zip = openZip(m_rootPath, QuaZip::mdAdd);
    }

    QuaZipFile file(zip.data());
    QuaZipNewInfo info(data->path + data->filename);

    qDebug() << "Setting relative path:" << relPath;

    if (!file.open(QIODevice::WriteOnly, info)) {
        qDebug() << "Failed to open file for writing in zip:" << relPath
                 << "Error:" << file.getZipError();
        return false;
    } 
    file.write(data->data);
    file.close();
    zip->close();
    return true;
}

// Remove files from the zip archive
// usage: bool success = zip.remove({"path/to/file1.txt", "path/to/file2.txt"});
bool PZip::remove(const QStringList &itemsToRemove) {
    QSharedPointer<QuaZip> openedZip = openZip(m_rootPath, QuaZip::mdUnzip);

    // Create a temporary file that will be the end output
    QString temp(m_rootPath + ".tmp");
    QSharedPointer<QuaZip> outZip = openZip(temp, QuaZip::mdCreate);

    QuaZipFile fileToCopy(openedZip.get());

    // copy all files except the ones to remove
    for (bool next = openedZip->goToFirstFile(); next; next = openedZip->goToNextFile()) {
        // start by getting the current file name (could be a directory so will need to check)
        QString currentFileName = openedZip->getCurrentFileName();

        // check if file is in our list of files to remove. if yes, skip.
        if (itemsToRemove.contains(currentFileName)) {
            qDebug() << "Removing file from zip:" << currentFileName;
            continue;
        }

        // if not, copy the file to the new zip file
        if (!fileToCopy.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to open file for reading in zip:" << currentFileName;
            continue;
        }

        // create a new file in the new zip with the same name as the current file
        QuaZipNewInfo newFileInfo(currentFileName);
        QuaZipFile newFile(outZip.get());
        if (!newFile.open(QIODevice::WriteOnly, newFileInfo)) {
            qDebug() << "Failed to open file for writing in zip:" << currentFileName;
            continue;
        }

        // read the data from the current file and write it to the new file
        QByteArray data = fileToCopy.readAll();
        newFile.write(data);
        newFile.close();
        fileToCopy.close();
    }

    // close the zip files
    openedZip->close();
    outZip->close();

    // remove the original zip file
    QFile removeFile(m_rootPath);
    if (!removeFile.remove()) {
        qDebug() << "Failed to remove original zip file:" << m_rootPath << "Error:" << removeFile.errorString();

        // remove the temporary zip file
        if (!QFile::remove(temp)) {
            qDebug() << "Failed to remove temporary zip file:" << temp;
        }
        removeFile.close();
        return false;
    }
    removeFile.close();

    // rename the temporary zip file to the original zip file name
    if (!QFile::rename(temp, m_rootPath)) {
        qDebug() << "Failed to rename temporary zip file:" << temp << "to" << m_rootPath;
        return false;
    }
    qDebug() << "Successfully removed files from zip:" << m_rootPath;
    return true;
}


// Remove a single file from the zip archive
// usage: bool success = zip.remove("path/to/file.txt");
bool PZip::remove(const QString &itemToRemove) {
    QStringList itemsToRemove;
    itemsToRemove.append(itemToRemove);
    return remove(itemsToRemove);
}

// Check if a file exists in the zip archive
bool PZip::exists(const QString &relFilePath) {
    if (read(relFilePath)->data.isEmpty()) {
        qDebug() << "File does not exist in zip:" << relFilePath;
        return false;
    }
    qDebug() << "File exists in zip:" << relFilePath;
    return true;
}

// Move a file in the zip archive
// usage: bool success = zip.move("path/to/file.txt", "new/path/to/file.txt");
// note: this will copy the file to the new location and remove the old one
bool PZip::move(const QString &filePath, const QString &newLocation) {
    QSharedPointer<PFileData> updatedFile = read(filePath);
    updatedFile->path = newLocation;
    if (!write(updatedFile)) {
        qDebug() << "Failed to write file to zip:" << filePath;
        return false;
    }
    return remove(filePath);
}

// Copy a file in the zip archive
// usage: bool success = zip.copy("path/to/file.txt", "new/path/to/file.txt");
// note: this will not remove the old file and will return true if the file already exists
bool PZip::copy(const QString &filePath, const QString &newLocation) {

    // if the two paths are the same, return true
    if (filePath == newLocation) {
        qDebug() << "File paths are the same, no need to copy:" << filePath;
        return true;
    }

    QSharedPointer<QuaZip> zip = openZip(m_rootPath, QuaZip::mdUnzip);
    QSharedPointer<QuaZipFile> file = openZipFile(zip, "", QIODevice::ReadOnly);

    QSharedPointer<PFileData> fileData = QSharedPointer<PFileData>::create();
    QByteArray data = file->readAll();
    fileData->data = data;
    fileData->filename = file->getFileName();
    fileData->ext = file->getFileName().section('.', -1, -1);
    fileData->path = file->getFileName().section('/', 0, -2);
    file->close();
    zip->close();

    return write(fileData);
}

bool PZip::rename(const QString &filePath, const QString &newFileName) {
    return move(filePath, newFileName);
}
