#include <QtTest/QtTest>
#include "../src/PConfigMgr.h"

class PTestConfigMgr : public QObject
{
    Q_OBJECT
private slots:
    void testLoadConfig_data();
    void testLoadConfig();
    // void testSaveConfig_data();
    // void testSaveConfig();
    // void testGetValue_data();
    // void testGetValue();
    // void testSetValue_data();
    // void testSetValue();
};

// Statics
QString testDataDir = QFINDTESTDATA("test_data/");

void PTestConfigMgr::testLoadConfig_data()
{
    QTest::addColumn<QString>("filePath");
    QTest::addColumn<bool>("expected");

    QTest::newRow("ini config") << testDataDir + "config.ini" << true;
    QTest::newRow("toml config") << testDataDir + "config.toml" << true;
    QTest::newRow("invalid config") << testDataDir + "config_invalid.ini" << false;
    QTest::newRow("empty config") << testDataDir + "config_empty.ini" << false;
}
void PTestConfigMgr::testLoadConfig()
{
    QFETCH(QString, filePath);
    QFETCH(bool, expected);

    PConfigMgr configMgr;
    bool result = configMgr.loadConfig(filePath);

    if (expected) {
        QVERIFY(result);
    } else {
        QVERIFY(!result);
    }
}

// void PTestConfigMgr::testGetValue_data()
// {
//     QTest::addColumn<QString>("section");
//     QTest::addColumn<QString>("key");
//     QTest::addColumn<QString>("expected");

//     QTest::newRow


QTEST_MAIN(PTestConfigMgr)
#include "PTestConfigMgr.moc"
