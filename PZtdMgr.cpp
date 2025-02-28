#include "PZtdMgr"

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

bool PZtdMgr::addFileToZtd(const QString &ztdFilePath, const QString &filePathToAdd) 
{
    // Open the ztd file
    QuaZip zip(ztdFilePath);
    if (!zip.open(QuaZip::mdAppend)) {
        return false; // Failed to open ztd file
    }

    // Add the file to the ztd
    QuaZipFile file(&zip);
    if (!file.open(QIODevice::WriteOnly, QuaZipNewInfo(filePathToAdd))) {
        zip.close();
        return false; // Failed to open file in ztd (in stream)
    }

    // Read the file to add and write it to the ztd
    QFile sourceFile(filePathToAdd);
    if (!sourceFile.open(QIODevice::ReadOnly)) {
        file.close();
        zip.close();
        return false; // Failed to open source file
    }

    QByteArray data = sourceFile.readAll();
    file.write(data);
    
    // Close the files
    sourceFile.close();
    file.close();
    zip.close();

    return true;
}

// TODO: Find more efficient method of removing files from ztd

// Moves a ztd file from one location on disk to another
bool PZtdMgr::moveZtdFile(const QString &ztdFilePath, const QString &newLocation) 
{
    // Check if the ztd file exists
    if (!QFile::exists(ztdFilePath)) {
        return false; // Ztd file does not exist
    }

    // Move the ztd file to the new location
    return QFile::rename(ztdFilePath, newLocation);
}

// Renames a ztd file
bool PZtdMgr::renameZtdFile(const QString &oldFilePath, const QString &newFileName) 
{
    // Check if the ztd file exists
    if (!QFile::exists(oldFilePath)) {
        return false; // Ztd file does not exist
    }

    // Get the directory of the old file
    QString dirPath = QFileInfo(oldFilePath).absolutePath();
    QString newFilePath = dirPath + "/" + newFileName;

    // Rename the ztd file
    return QFile::rename(oldFilePath, newFilePath);
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

    Quazip zip(filePath);
    // Check if empty
    if (!zip.goToFirstFile()) {
        zip.close();
        return false; // Empty file
    }

    // Is it a zip file?
    if (!zip.open(QuaZip::mdUnzip)) {
        zip.close();
        return false; // Not a zip file
    }

    // Verify it has ztd extension
    return filePath.endsWith(".ztd");
}

// Opens a file inside a ztd and returns its data
bool PZtdMgr::openFileInZtd(const QString &ztdFilePath, const QString &fileNameToOpen, QByteArray &fileData) 
{
    // Open the ztd file
    QuaZip zip(ztdFilePath);
    if (!zip.open(QuaZip::mdUnzip)) {
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
