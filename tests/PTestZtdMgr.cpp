#include <QtTest/QtTest>
#include "../PZtdMgr.h"

class PTestZtdMgr : public QObject
{
    Q_OBJECT
private slots:
    void testIsZtdFile_data();
    void testIsZtdFile();
    void testAddFileToZtd_data();
    void testAddFileToZtd();
    void testMoveZtdFile_data();
    void testMoveZtdFile();
    void testRenameZtdFile_data();
    void testRenameZtdFile();
    void testCopyZtdFile_data();
    void testCopyZtdFile();
    void testOpenFileInZtd_data();
    void testOpenFileInZtd();
};

// Statics
QString testDataDir = QFINDTESTDATA("test_data/");

void PTestZtdMgr::testIsZtdFile_data()
{
    QTest::addColumn<QString>("zipFilePath");
    QTest::addColumn<bool>("expectedResult");


    // Test case 1: Valid ztd file
    QTest::newRow("valid ztd file") << testDataDir + "valid.ztd" << true;

    // Test case 2: Invalid ztd file (not a zip)
    QTest::newRow("invalid ztd file") << testDataDir + "invalid.ztd" << false;

    // Test case 3: Invalid ztd file (no ztd extension)
    QTest::newRow("invalid ztd file no extension") << testDataDir + "invalid_no_ext" << false;

    // Test case 4: Invalid ztd file (empty)
    QTest::newRow("invalid ztd file empty") << testDataDir + "empty.ztd" << false;

    // Test case 3: Non-existent file
    QTest::newRow("non-existent file") << testDataDir + "nonexistent.ztd" << false;
}

void PTestZtdMgr::testIsZtdFile()
{
    QFETCH(QString, zipFilePath);
    QFETCH(bool, expectedResult);

    int result = PZtdMgr::isZtdFile(zipFilePath);
    QCOMPARE(result, expectedResult);
}

void PTestZtdMgr::testAddFileToZtd_data()
{
    QTest::addColumn<QString>("ztdFilePath");
    QTest::addColumn<QString>("filePathToAdd");
    QTest::addColumn<bool>("expectedResult");

    // Test case 1: Valid ztd file and valid file to add
    QTest::newRow("valid ztd and file") << testDataDir + "valid.ztd" << testDataDir + "new_file.txt" << true;

    // Test case 2: Valid ztd file and non-existent file to add
    QTest::newRow("valid ztd and non-existent file") << testDataDir + "valid.ztd" << testDataDir + "nonexistent.txt" << false;

    // Test case 3: Non-existent ztd file and valid file to add
    QTest::newRow("non-existent ztd and file") << testDataDir + "nonexistent.ztd" << testDataDir + "new_file.txt" << false;

    // Test case 4: Non-existent ztd file and non-existent file to add
    QTest::newRow("non-existent ztd and non-existent file") << testDataDir + "nonexistent.ztd" << testDataDir + "nonexistent.txt" << false;
}

void PTestZtdMgr::testAddFileToZtd()
{
    QFETCH(QString, ztdFilePath);
    QFETCH(QString, filePathToAdd);
    QFETCH(bool, expectedResult);

    // Run add function
    bool result = PZtdMgr::addFileToZtd(ztdFilePath, filePathToAdd);

    // Check expected result
    QCOMPARE(result, expectedResult);

    // Verify file was added successfully
    if (expectedResult) {
        QuaZip zip(ztdFilePath);
        QVERIFY(zip.open(QuaZip::mdUnzip));

        bool found = false;
        QString expectedFileName = QFileInfo(filePathToAdd).fileName();  // Extract the correct file name

        while (zip.goToNextFile()) {
            QString fileName = zip.getCurrentFileName();
            if (fileName == expectedFileName) {  // Compare only the filename
                found = true;
                break;
            }
        }

        QVERIFY(found);

        zip.close();
    }
}

void PTestZtdMgr::testMoveZtdFile_data()
{
    QTest::addColumn<QString>("ztdFilePath");
    QTest::addColumn<QString>("newLocation");
    QTest::addColumn<bool>("expectedResult");

    // Test case 1: Provided ztd file does not exist
    QTest::newRow("non-existent ztd file") << testDataDir + "nonexistent.ztd" << testDataDir + "test_dir/nonexistent.ztd" << false;

    // Test case 2: Moving ztd file to non-existent directory
    QTest::newRow("non-existent directory") << testDataDir + "valid.ztd" << testDataDir + "nonexistent_dir/valid.ztd" << false;

    // Test case 3: Moving ztd file to existing directory
    QTest::newRow("existing directory") << testDataDir + "valid.ztd" << testDataDir + "test_dir/valid.ztd" << true;
}

void PTestZtdMgr::testMoveZtdFile()
{
    QFETCH(QString, ztdFilePath);
    QFETCH(QString, newLocation);
    QFETCH(bool, expectedResult);

    // Ensure source file exists before testing
    QFile sourceFile(ztdFilePath);
    if (!sourceFile.exists()) {
        QVERIFY(expectedResult == false);
        return;
    }

    // Run move function
    bool result = PZtdMgr::moveZtdFile(ztdFilePath, newLocation);

    // Copy the file back to original location if move was successful
    if (result) {
        QVERIFY(QFile::copy(newLocation, ztdFilePath));
    }

    // Check expected result
    QCOMPARE(result, expectedResult);

    // Verify file was moved successfully
    if (expectedResult) {
        QFile movedFile(newLocation);
        QVERIFY(movedFile.exists());
        QVERIFY(sourceFile.size() == movedFile.size());

        // Clean up: remove moved file after test
        movedFile.remove();
    }
}

void PTestZtdMgr::testCopyZtdFile_data()
{
    QTest::addColumn<QString>("ztdFilePath");
    QTest::addColumn<QString>("ztdOutputCopyPath");
    QTest::addColumn<bool>("expectedResult");

    // Test case 1: Provided ztd file does not exist
    QTest::newRow("non-existent ztd file") << testDataDir + "nonexistent.ztd" << testDataDir + "nonexistent_copy.ztd" << false;

    // Test case 2: Copying ztd file to non-existent directory
    QTest::newRow("non-existent directory") << testDataDir + "valid.ztd" << testDataDir + "nonexistent_dir/valid.ztd" << false;

    // Test case 3: Copying ztd file to existing directory
    QTest::newRow("existing directory") << testDataDir + "valid.ztd" << testDataDir + "valid_copy.ztd" << true;
}

void PTestZtdMgr::testRenameZtdFile_data()
{
    QTest::addColumn<QString>("oldFilePath");
    QTest::addColumn<QString>("newFileName");
    QTest::addColumn<bool>("expectedResult");

    // Test case 1: Provided ztd file does not exist
    QTest::newRow("non-existent ztd file") << testDataDir + "nonexistent.ztd" << "nonexistent.ztd" << false;

    // Test case 2: Renaming ztd file to existing directory
    QTest::newRow("existing directory") << testDataDir + "valid.ztd" << "valid_rename.ztd" << true;
}

void PTestZtdMgr::testRenameZtdFile()
{
    QFETCH(QString, oldFilePath);
    QFETCH(QString, newFileName);
    QFETCH(bool, expectedResult);

    // Ensure source file exists before testing
    QFile sourceFile(oldFilePath);
    if (!sourceFile.exists()) {
        QVERIFY(expectedResult == false);
        return;
    }

    // Run rename function
    bool result = PZtdMgr::renameZtdFile(oldFilePath, newFileName);

    // Check expected result
    QCOMPARE(result, expectedResult);

    // Verify file was renamed successfully
    if (expectedResult) {
        QString newFilePath = QFileInfo(oldFilePath).absolutePath() + "/" + newFileName;
        QFile renamedFile(newFilePath);
        QVERIFY(renamedFile.exists());
        QVERIFY(sourceFile.size() == renamedFile.size());

        // Clean up: remove renamed file after test
        renamedFile.remove();
    }
}

void PTestZtdMgr::testCopyZtdFile()
{
    QFETCH(QString, ztdFilePath);
    QFETCH(QString, ztdOutputCopyPath);
    QFETCH(bool, expectedResult);

    // Ensure source file exists before testing
    QFile sourceFile(ztdFilePath);
    if (!sourceFile.exists()) {
        QVERIFY(expectedResult == false);
        return;
    }

    // Run copy function
    bool result = PZtdMgr::copyZtdFile(ztdFilePath, ztdOutputCopyPath);

    // Check expected result
    QCOMPARE(result, expectedResult);

    // Verify file was copied successfully
    QFile copiedFile(ztdOutputCopyPath);
    if (expectedResult) {
        QVERIFY(copiedFile.exists());
        QVERIFY(sourceFile.size() == copiedFile.size());

        // Clean up: remove copied file after test
        copiedFile.remove();
    }
}

void PTestZtdMgr::testOpenFileInZtd_data()
{
    QTest::addColumn<QString>("ztdFilePath");
    QTest::addColumn<QString>("fileNameToOpen");
    QTest::addColumn<bool>("expectedResult");

    // Test case 1: Provided ztd file does not exist
    QTest::newRow("non-existent ztd file") << testDataDir + "nonexistent.ztd" << "nonexistent.txt" << false;

    // Test case 2: Provided ztd file exists but file does not
    QTest::newRow("non-existent file") << testDataDir + "valid.ztd" << "nonexistent.txt" << false;

    // Test case 3: Provided ztd file and file exist
    QTest::newRow("valid ztd and file") << testDataDir + "valid.ztd" << "new_file.txt" << true;
}

void PTestZtdMgr::testOpenFileInZtd()
{
    QFETCH(QString, ztdFilePath);
    QFETCH(QString, fileNameToOpen);
    QFETCH(bool, expectedResult);

    // Make sure the source ztd file exists
    QFile sourceFile(ztdFilePath);
    if (!sourceFile.exists()) {
        QVERIFY(expectedResult == false);
        return;
    }

    // Run the function
    QByteArray fileData;
    bool result = PZtdMgr::openFileInZtd(ztdFilePath, fileNameToOpen, fileData);

    // Validate the function result
    QCOMPARE(result, expectedResult);

    // Validate the file data
    QString expectedFileData = "test";
    if (expectedResult) {
        QVERIFY(fileData == expectedFileData.toUtf8());
    }

    // If the file was expected to be found, verify its data
    if (expectedResult) {
        QVERIFY(!fileData.isEmpty());
    }
}


QTEST_MAIN(PTestZtdMgr)
#include "PTestZtdMgr.moc"
