#include "PZtdMgr.h"

PZtdMgr::PZtdMgr() 
{
}

PZtdMgr::~PZtdMgr() 
{
}

// Validates that a zip file contains ztd files
// TODO: validate the ztd files and handle cases with multiple ztd files
bool PZtdMgr::hasZtdFiles(const QString &zipFilePath) 
{
    // Traverse the zip file and check for ztd files
    QuaZip zip(zipFilePath);

    // Try to open the zip file (in dir)
    if (!zip.open(QuaZip::mdUnzip)) {
        return false; // Failed to open zip file
    }

    // Get list of files in the zip
    zip.setCurrentFile(zip.getZipName());
    QuaZipFile file(&zip);
    if (!file.open(QIODevice::ReadOnly)) {
        return false; // Failed to open file in zip (in stream)
    }

    // Check for ztd files
    bool hasZtd = false;
    while (zip.goToNextFile()) {
        QString fileName = zip.getCurrentFileName();
        if (fileName.endsWith(".ztd")) {
            hasZtd = true;
            break; // Found a ztd file
        }
    }

    zip.close();
    return hasZtd;
}

// TODO: Find more efficient method of adding files to ztd?
// TODO: Fix bug where it adds a duplicate file if it already exists
bool PZtdMgr::addFileToZtd(const QString &ztdFilePath, const QString &filePathToAdd) 
{
    if (!PZtdMgr::isZtdFile(ztdFilePath)) {
        qDebug() << "addFileToZtd: Not a valid ZTD file: " << ztdFilePath;
        return false; // Not a valid ZTD file
    }

    QuaZip zip(ztdFilePath);
    if (!zip.open(QuaZip::mdAdd)) {
        zip.close();
        qDebug() << "addFileToZtd: Failed to open ZTD file: " << zip.getZipError();
        return false; // Failed to open ZTD file
    }

    // Check if input file exists
    // NOTE: Need to address when file is in memory vs on disk
    // if (!QFile::exists(filePathToAdd)) {
    //     zip.close();
    //     return false; // File does not exist
    // }

    QuaZipFile outputFile(&zip);
    QuaZipNewInfo newFileInfo(QFileInfo(filePathToAdd).fileName(), filePathToAdd);
    if (!outputFile.open(QIODevice::WriteOnly, newFileInfo)) {
        zip.close();
        qDebug() << "addFileToZtd: Failed to open output file: " << outputFile.getZipError();
        return false; // Failed to open output file
    }

    QFile inputFile(filePathToAdd);
    if (!inputFile.open(QIODevice::ReadOnly)) {
        outputFile.close();
        zip.close();
        qDebug() << "addFileToZtd: Failed to open input file: " << filePathToAdd;
        return false; // Failed to open input file
    }

    QByteArray data = inputFile.readAll();
    inputFile.close();

    outputFile.write(data);
    outputFile.close();
    zip.close();

    return true;
}

// Extracts all files from a ztd file to a temporary directory
bool PZtdMgr::extractFilesFromZtd(const QString &ztdFilePath, const QString &tempDirPath)
{
    QuaZip zip(ztdFilePath);
    if (!zip.open(QuaZip::mdUnzip)) {
        qDebug() << "Failed to open ZTD file for extraction: " << zip.getZipError();
        return false;
    }

    QDir tempDir(tempDirPath);
    if (!tempDir.exists()) {
        tempDir.mkpath(".");
    }

    for (bool more = zip.goToFirstFile(); more; more = zip.goToNextFile()) {
        QString fileName = zip.getCurrentFileName();
        QString fullPath = tempDir.filePath(fileName);

        QuaZipFile zipFile(&zip);
        if (!zipFile.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to open file in ZTD: " << fileName;
            continue;
        }

        QFile outFile(fullPath);
        if (!outFile.open(QIODevice::WriteOnly)) {
            qDebug() << "Failed to create temp file: " << fullPath;
            zipFile.close();
            continue;
        }

        outFile.write(zipFile.readAll());
        zipFile.close();
        outFile.close();
    }

    zip.close();
    return true;
}

// Replaces a file in a directory with new data
bool PZtdMgr::replaceFileInDir(const QString &tempDirPath, const QString &filePathToAdd)
{
    QString fileName = QFileInfo(filePathToAdd).fileName();
    QString destPath = QDir(tempDirPath).filePath(fileName);

    if (QFile::exists(destPath)) {
        QFile::remove(destPath);
    }

    return QFile::copy(filePathToAdd, destPath);
}

// Creates a ztd file from a directory
bool PZtdMgr::createZtdFromDir(const QString &ztdFilePath, const QString &tempDirPath)
{
    // Ensure parent directory exists
    QFileInfo ztdFileInfo(ztdFilePath);
    QDir parentDir = ztdFileInfo.absoluteDir();
    if (!parentDir.exists()) {
        qDebug() << "Creating missing directory:" << parentDir.absolutePath();
        if (!parentDir.mkpath(".")) {
            qDebug() << "Failed to create parent directory!";
            return false;
        }
    }

    // Ensure temp directory exists
    QDir tempDir(tempDirPath);
    if (!tempDir.exists()) {
        qDebug() << "Temp directory does not exist: " << tempDirPath;
        return false;
    }

    // Delete old ZTD if it exists
    if (QFile::exists(ztdFilePath)) {
        QFile::remove(ztdFilePath);
    }

    QString fileName = QFileInfo(ztdFilePath).fileName();
    QuaZip newZip(fileName);
    if (!newZip.open(QuaZip::mdCreate)) {
        qDebug() << "Failed to create new ZTD file: " << newZip.getZipError();
        return false;
    }

    // Get file list with proper filtering
    QStringList fileList = tempDir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    for (const QString &fileName : fileList) {
        QString fullPath = tempDir.filePath(fileName);

        QuaZipFile newFile(&newZip);
        if (!newFile.open(QIODevice::WriteOnly, QuaZipNewInfo(fileName))) {
            qDebug() << "Failed to write file to ZTD: " << fileName << " Error:" << newFile.getZipError();
            continue;
        }

        QFile file(fullPath);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to open temp file: " << fullPath;
            newFile.close();
            continue;
        }

        newFile.write(file.readAll());
        file.close();
        newFile.close();
    }

    newZip.close();
    return true;
}


// Checks if a file exists in a ztd
bool PZtdMgr::fileExistsInZtd(const QString &ztdFilePath, const QString &fileNameToCheck)
{
    QuaZip zip(ztdFilePath);
    if (!zip.open(QuaZip::mdUnzip)) {
        qDebug() << "Failed to open ZTD for reading: " << zip.getZipError();
        zip.close();
        return false;
    }

    for (bool more = zip.goToFirstFile(); more; more = zip.goToNextFile()) {
        if (zip.getCurrentFileName() == fileNameToCheck) {
            zip.close();
            return true;
        }
    }

    zip.close();
    return false;
}

// TODO: Find more efficient method of removing files from ztd
bool PZtdMgr::removeFileFromZtd(const QString &ztdFilePath, const QString &fileNameToRemove)
{
    QString tempZtdPath = ztdFilePath + ".tmp";  // Temporary file
    bool fileRemoved = false;

    // Guarantee the temp file does not exist
    QFile::remove(tempZtdPath);

    QuaZip oldZip(ztdFilePath);
    QuaZip newZip(tempZtdPath);

    if (!oldZip.open(QuaZip::mdUnzip)) {
        qDebug() << "Failed to open ZTD file for reading: " << oldZip.getZipError();
        oldZip.close();
        newZip.close();
        return false;
    }

    if (!newZip.open(QuaZip::mdCreate)) {
        qDebug() << "Failed to create new ZTD file: " << newZip.getZipError();
        oldZip.close();
        newZip.close();
        return false;
    }

    // Validate ztd file
    if (!PZtdMgr::isZtdFile(ztdFilePath)) {
        qDebug() << "Not a valid ZTD file: " << ztdFilePath;
        oldZip.close();
        newZip.close();
        return false;
    }

    QuaZipFile oldFile(&oldZip);
    QuaZipFile newFile(&newZip);

    for (bool more = oldZip.goToFirstFile(); more; more = oldZip.goToNextFile()) {
        QString existingFileName = oldZip.getCurrentFileName();
        if (existingFileName == fileNameToRemove) {
            qDebug() << "Removing file from ZTD: " << fileNameToRemove;
            fileRemoved = true;
            continue;  // Skip this file (remove it)
        }

        if (!oldFile.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to read existing file: " << existingFileName;
            continue;
        }

        QByteArray data = oldFile.readAll();
        oldFile.close();

        if (!newFile.open(QIODevice::WriteOnly, QuaZipNewInfo(existingFileName))) {
            qDebug() << "Failed to write existing file: " << existingFileName;
            continue;
        }

        newFile.write(data);
        newFile.close();
    }

    // Close handles if they are open
    if (oldZip.isOpen()) {
        oldZip.close();
    }
    if (newZip.isOpen()) {
        newZip.close();
    }

    // Remove the old archive
    if (!QFile::remove(ztdFilePath)) {
        qDebug() << "Failed to remove old ZTD file. Exists?" << QFile::exists(ztdFilePath);
        return false;
    }

    // Replace the old archive with the new one
    if (!QFile::rename(tempZtdPath, ztdFilePath)) {
        qDebug() << "Failed to replace old ZTD file with new one";
        qDebug() << "Temp ZTD: " << tempZtdPath;
        qDebug() << "New ZTD: " << ztdFilePath;
        return false;
    }

    return fileRemoved;
}

// Moves a ztd file from one location on disk to another
bool PZtdMgr::moveFile(const QString &filePath, const QString &newLocation)
{
    QFileInfo sourceInfo(filePath);
    QFileInfo destInfo(newLocation);
    
    // check if dest exists
    QDir destDir = destInfo.dir();
    if (!destDir.exists()) {
        destDir.mkpath(".");
    }
    
    // does file exist
    if (!sourceInfo.exists() || !sourceInfo.isFile()) {
        qDebug() << "Source file doesn't exist or isn't a file:" << filePath;
        return false;
    }
    
    // try classic rename to move file ( same drive solution)
    if (QFile::rename(filePath, newLocation)) {
        return true;
    }
    
    // try copy and delte (diff drives)
    if (QFile::copy(filePath, newLocation)) {
        // Ensure copy was successful before deleting source
        if (QFile::exists(newLocation)) {
            if (QFile::remove(filePath)) {
                return true;
            } else {
                qDebug() << "Copied file but couldn't remove source:" << filePath;
                return true; 
            }
        }
    }
    
    qDebug() << "Failed to move file:" << filePath << "to" << newLocation;
    return false;
}

bool PZtdMgr::copyZtdFile(const QString &ztdFilePath, const QString &ztdOutputCopyPath) 
{
    // Check if the ztd file exists
    if (!QFile::exists(ztdFilePath)) {
        return false; // Ztd file does not exist
    }

    // Copy the ztd file to the new location
    return QFile::copy(ztdFilePath, ztdOutputCopyPath);
}

// Validates if a file is a ztd file and is readable
bool PZtdMgr::isZtdFile(const QString &filePath)
{
    // Does file exist?
    if (!QFile::exists(filePath)) {
        return false; // File does not exist
    }

    // Make sure it has a .ztd extension first
    if (!filePath.endsWith(".ztd")) {
        return false; // Wrong extension
    }

    QuaZip zip(filePath);
    // Is it a zip file? (and open file)
    // 1. Probably empty
    // 2. Not a valid zip file
    if (!zip.open(QuaZip::mdUnzip)) {
        return false; // Not a zip file
    }

    // Verify it has ztd extension
    zip.close();
    return true;
}

// Opens a file inside a ztd and returns its data
bool PZtdMgr::openFileInZtd(const QString &ztdFilePath, const QString &fileNameToOpen, QByteArray &fileData) 
{
    // Open the ztd file
    QuaZip zip(ztdFilePath);
    if (!zip.open(QuaZip::mdUnzip)) {
        zip.close();
        return false; // Failed to open ztd file
    }

    // Set the current file to the one we want to open
    if (!zip.setCurrentFile(fileNameToOpen)) {
        zip.close();
        return false; // Failed to set current file
    }

    // Open the file inside the ztd
    QuaZipFile file(&zip);
    if (!file.open(QIODevice::ReadOnly)) {
        file.close();
        zip.close();
        return false; // Failed to open file in ztd (in stream)
    }

    // Read the data from the file
    fileData = file.readAll();

    // Close the files
    file.close();
    zip.close();

    return true;
}

// Removes a file from a directory
bool PZtdMgr::removeFileFromDir(const QString &file) 
{
    // Check if file exists
    if (!QFile::exists(file)) {
        return false; // File does not exist
    }

    // Remove the file
    return QFile::remove(file);
}

// Gets a list of files in a ztd file with a specific extension and level
QList<PZtdMgr::FileData> PZtdMgr::getFilesInZtd(const QString &ztdFilePath, const QString &ext, int maxLevel, const QStringList &folderList, QuaZip *zip)
{
    QList<PZtdMgr::FileData> filesFound;
    bool createdZip = false;

    if (!zip) {
        zip = new QuaZip(ztdFilePath);
        createdZip = true;

        if (!zip->open(QuaZip::mdUnzip)) {
            qDebug() << "Failed to open ZTD file for extraction: " << zip->getZipError();
            delete zip;
            return filesFound;
        }

        // get filenames
        QStringList rootEntries;
        QuaZipFileInfo info;
        QuaZipFile file(zip);
        for (bool more = zip->goToFirstFile(); more; more = zip->goToNextFile()) {
            zip->getCurrentFileInfo(&info);
            rootEntries << info.name;
        }

        filesFound.append(getFilesInZtd(ztdFilePath, ext, maxLevel, rootEntries, zip));

        if (createdZip) {
            zip->close();
            delete zip;
        }
        return filesFound;
    }

    if (maxLevel > 0) {
        QStringList subFolders;

        for (const QString &entry : folderList) {
            zip->setCurrentFile(entry);
            QuaZipFileInfo info;
            zip->getCurrentFileInfo(&info);

            if (info.name.endsWith('/') && QFileInfo(info.name).isDir()) {
                // if folder, get all files in the folder
                for (bool more = zip->goToFirstFile(); more; more = zip->goToNextFile()) {
                    QuaZipFileInfo info;
                    zip->getCurrentFileInfo(&info);

                    if (info.name.startsWith(entry) && info.name != entry) {
                        subFolders << info.name;
                    }
                }
            } else {
                // found file, check if it matches the extension
                if (entry.endsWith(ext)) {
                    QuaZipFile zipFile(zip);
                    if (!zip->setCurrentFile(entry)) continue;

                    if (!zipFile.open(QIODevice::ReadOnly)) continue;

                    QByteArray fileData = zipFile.readAll();
                    FileData file;
                    file.data = fileData;
                    file.filename = QFileInfo(entry).fileName();
                    file.ext = QFileInfo(entry).suffix();
                    file.path = entry;
                    filesFound.append(file);
                    zipFile.close();
                }
            }
        }

        if (!subFolders.isEmpty()) {
            filesFound.append(getFilesInZtd(ztdFilePath, ext, maxLevel - 1, subFolders, zip));
        }
    }

    return filesFound;
}

// Gets a file from a relative path in a ztd file
QByteArray PZtdMgr::getFileFromRelPath(const QString &ztdFilePath, const QString &relPath) 
{
    QuaZip zip(ztdFilePath);
    if (!zip.open(QuaZip::mdUnzip)) {
        qWarning() << "Failed to open ZTD file:" << ztdFilePath;
        return {};
    }

    if (!zip.setCurrentFile(relPath)) {
        qWarning() << "File not found in ZTD:" << relPath;
        zip.close();
        return {};
    }

    QuaZipFile file(&zip);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file in ZTD:" << relPath;
        zip.close();
        return {};
    }

    QByteArray data = file.readAll();
    file.close();
    zip.close();

    return data;
}

// Delete a file from the filesystem
bool PZtdMgr::deleteFile(const QString &filePath) 
{
    if (QFile::exists(filePath)) {
        return QFile::remove(filePath);
    }
    return false;
}

// Rename a file on the filesystem
bool PZtdMgr::renameFile(const QString &filePath, const QString &newFileName) 
{
    if (QFile::exists(filePath)) {
        QString dirPath = QFileInfo(filePath).absolutePath();
        QString newFilePath = dirPath + "/" + newFileName;
        return QFile::rename(filePath, newFilePath);
    }
    return false;
}

// Create a directory on the filesystem
bool PZtdMgr::makeDir(const QString &dirPath) 
{
    QDir dir(dirPath);
    if (!dir.exists()) {
        return dir.mkpath(dirPath);
    }
    return false;
}