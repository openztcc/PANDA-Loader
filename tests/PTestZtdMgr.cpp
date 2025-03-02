#include <QtTest/QtTest>
#include "../PZtdMgr.h"

class PTestZtdMgr : public QObject
{
    Q_OBJECT
private slots:
    void testIsZtdFile_data();
    void testIsZtdFile();
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

QTEST_MAIN(PTestZtdMgr)
#include "PTestZtdMgr.moc"
