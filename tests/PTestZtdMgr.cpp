#include <QtTest/QtTest>
#include "../PZtdMgr.h"

class PTestZtdMgr : public QObject
{
    Q_OBJECT
private slots:
    void testHasZtdFiles_data();
    void testHasZtdFiles();
    // void testAddFileToZtd_data();
    // void testAddFileToZtd();
    // void testMoveZtdFile_data();
    // void testMoveZtdFile();
    // void testRenameZtdFile_data();
    // void testRenameZtdFile();
    // void testCopyZtdFile_data();
    // void testCopyZtdFile();
    // void testOpenFileInZtd_data();
    // void testOpenFileInZtd();
};

// Statics
QString testDataDir = QFINDTESTDATA("testdata/");

void PTestZtdMgr::testHasZtdFiles_data()
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

void PTestZtdMgr::testHasZtdFiles()
{
    QFETCH(QString, zipFilePath);
    QFETCH(bool, expectedResult);

    bool result = PZtdMgr::hasZtdFiles(zipFilePath);
    QVERIFY (result == expectedResult);
}

QTEST_MAIN(PTestZtdMgr)
#include "PTestZtdMgr.moc"
