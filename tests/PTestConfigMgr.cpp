#include <QtTest/QtTest>
#include "../PConfigMgr.h"

class PTestConfigMgr : public QObject
{
    Q_OBJECT
private slots:
    void testGetMetaConfig_data();
    void testGetMetaConfig();
    // void testUpdateMetaConfig_data();
    // void testUpdateMetaConfig();
    // void testRemoveMetaConfig_data();
    // void testRemoveMetaConfig();
    // void testAddMetaConfig_data();
    // void testAddMetaConfig();
    // void testGetZooIniConfig_data();
    // void testGetZooIniConfig();
    // void testUpdateZooIniConfig_data();
    // void testUpdateZooIniConfig();
    // void testRemoveZooIniConfig_data();
    // void testRemoveZooIniConfig();
};

// Statics
QString testDataDir = QFINDTESTDATA("test_data/");

void PTestConfigMgr::testGetMetaConfig_data()
{
    QTest::addColumn<QString>("ztdFilePath");
    QTest::addColumn<bool>("expected");

    QTest::newRow("has valid meta.toml") << testDataDir + "config_valid.ztd" << true;
    QTest::newRow("does not have meta.toml") << testDataDir + "config_invalid.ztd" << false;
}

void PTestConfigMgr::testGetMetaConfig()
{
    QFETCH(QString, ztdFilePath);
    QFETCH(bool, expected);

    PConfigMgr configMgr;
    toml::table config = configMgr.getMetaConfig(ztdFilePath);
    
    if (expected) {
        QVERIFY(!config.empty());
    } else {
        QVERIFY(config.empty());
    }

    // print toml table
    for (const auto &[key, value] : config) {
        QString k = QString::fromStdString(static_cast<std::string>(key));
        QString v = PConfigMgr::getKeyValue(k, config);
        if (k == "authors" || k == "tags") {
            qDebug() << "[" << k << "] =";
            toml::array arr = value.as_array();
            for (const auto &item : arr) {
                qDebug() << "[" << k << "] = " << item.as_string()->get();
            }
            continue;
        } else if (k == "dependencies") {
            qDebug () << "[ Dependencies ] =";
            toml::table depTable = value.as_table();
            for (const auto &[depKey, depValue] : depTable) {
                QString depK = QString::fromStdString(static_cast<std::string>(depKey));
                QString depV = QString::fromStdString(depValue.as_string()->get());
                qDebug() << "[" << depKey << "] = " << depK << " : " << depV;
            }
            continue;
        }
        qDebug() << "[" << k << "] = " << v;
    }
}

QTEST_MAIN(PTestConfigMgr)
#include "PTestConfigMgr.moc"
