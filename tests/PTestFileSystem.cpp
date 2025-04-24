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
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QString>("relFilePath");
    QTest::addColumn<bool>("expectedData");

    QTest::newRow("Open a ZTD file.") << testDataDir << "nyala.ztd" << "animals/hwnyala.uca" << true;
    QTest::newRow("Open missing ZTD file.") << testDataDir << "missing.ztd" << "" << false;
}

void PTestFileSystem::testRead()
{
    QFETCH(QString, filePath);
    QFETCH(QString, fileName);
    QFETCH(QString, relFilePath);
    QFETCH(bool, expectedData);

    // Create a PFileSystem object
    PFile fileSystem(this, filePath + fileName, FileType::Zip);

    // Read the file
    PFileData fileData = fileSystem.read(relFilePath);

    // Check if the data is as expected
    if (expectedData) {
        QVERIFY(!fileData.data.isEmpty());
        QCOMPARE(fileData.filename, "hwnyala.uca");
        QCOMPARE(fileData.ext, "uca");
        QCOMPARE(fileData.path, "animals/hwnyala.uca");
    } else {
        QVERIFY(fileData.data.isEmpty());
    }
}

QTEST_MAIN(PTestFileSystem)
#include "PTestFileSystem.moc"
