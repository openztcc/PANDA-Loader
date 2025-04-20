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
    void testGetValue_INI_data();
    void testGetValue_INI();
    void testGetValue_TOML_data();
    void testGetValue_TOML();
    void testSetValue_INI_data();
    void testSetValue_INI();
    void testSetValue_TOML_data();
    void testSetValue_TOML();
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

// INI config test data
void PTestConfigMgr::testGetValue_INI_data()
{
    QTest::addColumn<QString>("section");
    QTest::addColumn<QString>("key");
    QTest::addColumn<QString>("expected");

    QTest::newRow("key: fullscreen") << "user" << "fullscreen" << "0";
    QTest::newRow("invalid key") << "user" << "invalid_key" << "";
    QTest::newRow("key: path") << "resource" << "path" << "./dlupdate;./updates;./xpack2/loc;./xpack2;./dupdate;./zupdate1;./xpack1/loc;./xpack1;./zupdate;./loc;"; 
    QTest::newRow("key: maxLongTooltipWidth") << "UI" << "maxLongTooltipWidth" << "400";
}
void PTestConfigMgr::testGetValue_INI()
{
    QFETCH(QString, section);
    QFETCH(QString, key);
    QFETCH(QString, expected);

    PConfigMgr configMgr;
    configMgr.loadConfig(testDataDir + "config.ini");
    QString result = configMgr.getValue(section, key).toString();
    qDebug() << "Result: " << result;
    qDebug() << "Expected: " << expected;

    if (expected.isEmpty()) {
        QVERIFY(result.isEmpty());
    } else {
        QCOMPARE(result, expected);
    }
}

// TOML config test data
void PTestConfigMgr::testGetValue_TOML_data()
{
    QTest::addColumn<QString>("section");
    QTest::addColumn<QString>("key");
    QTest::addColumn<QVariant>("expected");

    QTest::newRow("key: isoPath") << "" << "isoPath" << QVariant("F:\\Backup_041723\\Zoo Tycoon Complete Collection Disk 2.iso");
    QTest::newRow("invalid key") << "user" << "invalid_key" << QVariant();
    QTest::newRow("key: useIsoMounting") << "" << "useIsoMounting" << QVariant(true);
    QTest::newRow("key: zooGamePath") << "" << "zooGamePath" << QVariant("C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon");
}

void PTestConfigMgr::testGetValue_TOML()
{
    QFETCH(QString, section);
    QFETCH(QString, key);
    QFETCH(QVariant, expected);

    PConfigMgr configMgr;
    configMgr.loadConfig(testDataDir + "config.toml");

    QVariant result = configMgr.getValue(section, key);
    qDebug() << "Result: " << result;
    qDebug() << "Expected: " << expected;

    QCOMPARE(result, expected);
}

void PTestConfigMgr::testSetValue_INI_data()
{
    QTest::addColumn<QString>("key");
    QTest::addColumn<QString>("value");
    QTest::addColumn<QString>("section");

    QTest::newRow("key: fullscreen") << "fullscreen" << "1" << "user";
    QTest::newRow("invalid key") << "invalid_key" << "1" << "user";
    QTest::newRow("key: path") << "path" << "./dlupdate;." << "resource";
}

void PTestConfigMgr::testSetValue_INI()
{
    QFETCH(QString, section);
    QFETCH(QString, key);
    QFETCH(QString, value);

    PConfigMgr configMgr;
    configMgr.loadConfig(testDataDir + "config.ini");
    configMgr.setValue(key, value, section);

    QString result = configMgr.getValue(section, key).toString();
    qDebug() << "Result: " << result;
    qDebug() << "Expected: " << value;

    QCOMPARE(result, value);
}

void PTestConfigMgr::testSetValue_TOML_data()
{
    QTest::addColumn<QString>("key");
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<QString>("section");

    QTest::newRow("key: isoPath") << "isoPath" << QVariant("F:\\Backup_041723\\Zoo Tycoon Complete Collection Disk 3.iso") << "";
    QTest::newRow("invalid key") << "invalid_key" << QVariant("F:\\Backup_041723\\Zoo Tycoon Complete Collection Disk 2.iso") << "user";
    QTest::newRow("key: useIsoMounting") << "useIsoMounting" << QVariant(false) << "";
    QTest::newRow("key: zooGamePath") << "zooGamePath" << QVariant("C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon 2") << "";
}
void PTestConfigMgr::testSetValue_TOML()
{
    QFETCH(QString, section);
    QFETCH(QString, key);
    QFETCH(QVariant, value);

    PConfigMgr configMgr;
    configMgr.loadConfig(testDataDir + "config.toml");
    configMgr.setValue(key, value, section);

    QVariant result = configMgr.getValue(section, key);
    qDebug() << "Result: " << result;
    qDebug() << "Expected: " << value;

    QCOMPARE(result, value);
}


QTEST_MAIN(PTestConfigMgr)
#include "PTestConfigMgr.moc"
