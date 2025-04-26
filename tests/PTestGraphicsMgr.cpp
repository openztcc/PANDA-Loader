#include <QtTest/QtTest>
#include "PApeFile.h"

class PTestGraphicsMgr : public QObject
{
    Q_OBJECT
private slots:
    void testGetGraphicBuffers_data();
    void testGetGraphicBuffers();
    void testProcessIcons_data();
    void testProcessIcons();

};

// Statics
QString testDataDir = QFINDTESTDATA("test_data/");

void PTestGraphicsMgr::testGetGraphicBuffers_data()
{
    QTest::addColumn<QString>("ztdFilePath");
    QTest::addColumn<bool>("expected");

    QTest::newRow("has valid ucb") << testDataDir + "getfile_valid.ztd" << true;
    QTest::newRow("has valid uca") << testDataDir + "getfile_valid_uca.ztd" << true;
}

void PTestGraphicsMgr::testGetGraphicBuffers()
{
    QFETCH(QString, ztdFilePath);
    QFETCH(bool, expected);

    // Get graphic buffers
    QMap<QString, OutputBuffer> graphicBuffers = PApeFile::getGraphicBuffers(ztdFilePath);

    if (expected) {
        QVERIFY(!graphicBuffers.isEmpty());
        for (const auto &key : graphicBuffers.keys()) {
            qDebug() << "Graphic buffer:" << key;
        }
    } else {
        QVERIFY(graphicBuffers.isEmpty());
    }
}

void PTestGraphicsMgr::testProcessIcons_data()
{
    QTest::addColumn<QString>("ztdFilePath");
    QTest::addColumn<bool>("expected");

    QTest::newRow("has valid ucb") << testDataDir + "getfile_valid.ztd" << true;
    QTest::newRow("has valid uca") << testDataDir + "getfile_valid_uca.ztd" << true;
}

void PTestGraphicsMgr::testProcessIcons()
{
    QFETCH(QString, ztdFilePath);
    QFETCH(bool, expected);

    // Get graphic buffers
    QMap<QString, OutputBuffer> graphicBuffers = PApeFile::getGraphicBuffers(ztdFilePath);

    // Process icons
    QStringList processedIcons = PApeFile::processIcons(graphicBuffers);

    if (expected) {
        QVERIFY(!processedIcons.isEmpty());
        for (const auto &icon : processedIcons) {
            qDebug() << "Processed icon:" << icon;
        }
    } else {
        QVERIFY(processedIcons.isEmpty());
    }
}


QTEST_MAIN(PTestGraphicsMgr)
#include "PTestGraphicsMgr.moc"
