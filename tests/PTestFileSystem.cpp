#include <QtTest/QtTest>
#include "../src/ldrs/PFile.h"

class PTestFileSystem : public QObject
{
    Q_OBJECT
private slots:
    void testReadZip_data();
    void testReadZip();
    void testWriteZip_data();
    void testWriteZip();
    void testExistsZip_data();
    void testExistsZip();
    void testRemoveZip_data();
    void testRemoveZip();
};

// Statics
QString testDataDir = QFINDTESTDATA("test_data/filesystem/");

void PTestFileSystem::testReadZip_data()
{
    QTest::addColumn<QString>("filePath");
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QString>("relFilePath");
    QTest::addColumn<bool>("expectedData");

    QTest::newRow("Open a ZTD file.") << testDataDir << "nyala.ztd" << "animals/hwnyala.uca" << true;
    QTest::newRow("Open missing ZTD file.") << testDataDir << "missing.ztd" << "" << false;
}

void PTestFileSystem::testReadZip()
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
        QCOMPARE(fileData.path, "animals/");
    } else {
        QVERIFY(fileData.data.isEmpty());
    }
}

void PTestFileSystem::testWriteZip_data()
{
    QTest::addColumn<QString>("filePath");
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QString>("relFilePath");
    QTest::addColumn<bool>("expectedData");

    QTest::newRow("Write to a ZTD file.") << testDataDir << "nyala.ztd" << "animals/" << true;
    QTest::newRow("Write to missing ZTD file.") << testDataDir << "missing.ztd" << "" << false;
}

void PTestFileSystem::testWriteZip()
{
    QFETCH(QString, filePath);
    QFETCH(QString, fileName);
    QFETCH(QString, relFilePath);
    QFETCH(bool, expectedData);

    // Create a PFileSystem object
    PFile fileSystem(this, filePath + fileName, FileType::Zip);

    // Create a PFileData object to write
    PFileData fileData;
    QByteArray data;
    QFile file(filePath + "config.toml");
    if (file.open(QIODevice::ReadOnly)) {
        data = file.readAll();
        file.close();
    }
    fileData.data = data;
    fileData.filename = "config.toml";
    fileData.ext = "toml";
    fileData.path = relFilePath;

    // Write the file
    bool result = fileSystem.write(fileData);

    // Read the file back to check if it was written correctly
    PFileData readFileData = fileSystem.read(relFilePath + fileData.filename);

    // Check if the data is as expected
    if (expectedData) {
        QVERIFY(result);
        QCOMPARE(readFileData.data, fileData.data);
        QCOMPARE(readFileData.filename, fileData.filename);
        QCOMPARE(readFileData.ext, fileData.ext);
        QCOMPARE(readFileData.path, fileData.path);
        QVERIFY(!result);
    }
}

void PTestFileSystem::testExistsZip_data()
{
    QTest::addColumn<QString>("filePath");
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QString>("relFilePath");
    QTest::addColumn<bool>("expectedData");

    QTest::newRow("Check if file exists in ZTD.") << testDataDir << "nyala.ztd" << "animals/hwnyala.uca" << true;
    QTest::newRow("Check if file does not exist in ZTD.") << testDataDir << "nyala.ztd" << "animals/missing.uca" << false;
}

void PTestFileSystem::testExistsZip()
{
    QFETCH(QString, filePath);
    QFETCH(QString, fileName);
    QFETCH(QString, relFilePath);
    QFETCH(bool, expectedData);

    // Create a PFileSystem object
    PFile fileSystem(this, filePath + fileName, FileType::Zip);

    // Check if the file exists
    bool result = fileSystem.exists(relFilePath);

    // Check if the result is as expected
    QCOMPARE(result, expectedData);
}

void PTestFileSystem::testRemoveZip_data()
{
    QTest::addColumn<QString>("filePath");
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QString>("relFilePath");
    QTest::addColumn<bool>("expectedData");

    QTest::newRow("Remove file from ZTD.") << testDataDir << "nyala.ztd" << "animals/hwnyala.uca" << true;
    QTest::newRow("Remove missing file from ZTD.") << testDataDir << "nyala.ztd" << "animals/missing.uca" << false;
}

void PTestFileSystem::testRemoveZip()
{
    QFETCH(QString, filePath);
    QFETCH(QString, fileName);
    QFETCH(QString, relFilePath);
    QFETCH(bool, expectedData);

    // Create a PFileSystem object
    PFile fileSystem(this, filePath + fileName, FileType::Zip);

    // Remove the file
    bool result = fileSystem.remove(relFilePath);

    // Check if the result is as expected
    QCOMPARE(result, expectedData);
}

QTEST_MAIN(PTestFileSystem)
#include "PTestFileSystem.moc"
