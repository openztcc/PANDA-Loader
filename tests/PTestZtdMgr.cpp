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
    // void testMoveZtdFile_data();
    // void testMoveZtdFile();
    // void testRenameZtdFile_data();
    // void testRenameZtdFile();
    void testCopyZtdFile_data();
    void testCopyZtdFile();
    // void testOpenFileInZtd_data();
    // void testOpenFileInZtd();
};

// Statics
QString testDataDir = QFINDTESTDATA("test_data/");

void PTestZtdMgr::testIsZtdFile_data()
{
    QTest::addColumn<QString>("zipFilePath");
    QTest::addColumn<int>("expectedResult");


    // Test case 1: Valid ztd file
    QTest::newRow("valid ztd file") << testDataDir + "valid.ztd" << 0;

    // Test case 2: Invalid ztd file (not a zip)
    QTest::newRow("invalid ztd file") << testDataDir + "invalid.ztd" << 3;

    // Test case 3: Invalid ztd file (no ztd extension)
    QTest::newRow("invalid ztd file no extension") << testDataDir + "invalid_no_ext" << 2;

    // Test case 4: Invalid ztd file (empty)
    QTest::newRow("invalid ztd file empty") << testDataDir + "empty.ztd" << 3;

    // Test case 3: Non-existent file
    QTest::newRow("non-existent file") << testDataDir + "nonexistent.ztd" << 1;
}

void PTestZtdMgr::testIsZtdFile()
{
    QFETCH(QString, zipFilePath);
    QFETCH(int, expectedResult);

    int result = PZtdMgr::isZtdFile(zipFilePath);
    QCOMPARE (result, expectedResult);
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



QTEST_MAIN(PTestZtdMgr)
#include "PTestZtdMgr.moc"
