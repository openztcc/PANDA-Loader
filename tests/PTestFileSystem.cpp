#include <QtTest/QtTest>
#include "../src/ldrs/PFile.h"

class PTestFileSystem : public QObject
{
    Q_OBJECT
private slots:
    void testRead_data();
    void testRead();
};

// Statics
QString testDataDir = QFINDTESTDATA("test_data/filesystem/");

void PTestFileSystem::testRead_data()
{
    QTest::addColumn<QString>("filePath");
    QTest::addColumn<bool>("expectedData");

    QTest::newRow("Open a ZTD file.") << testDataDir + "nyala.ztd" << true;
    QTest::newRow("Open missing ZTD file.") << testDataDir + "missing.ztd" << false;
}

void PTestFileSystem::testRead()
{
    QFETCH(QString, filePath);
    QFETCH(bool, expectedData);

    // Create a PFileSystem object
    PFileSystem fileSystem;
    fileSystem.setRootPath(filePath);

    // Read the file
    PFileData fileData = fileSystem.read(filePath);

    // Check if the data is as expected
    if (expectedData) {
        QVERIFY(!fileData.data.isEmpty());
        QCOMPARE(fileData.filename, "nyala.ztd");
        QCOMPARE(fileData.ext, "ztd");
        QCOMPARE(fileData.path, "nyala.ztd");
    } else {
        QVERIFY(fileData.data.isEmpty());
    }
}

QTEST_MAIN(PTestFileSystem)
#include "PTestFileSystem.moc"
