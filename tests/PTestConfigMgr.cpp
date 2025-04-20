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

    toml::table config = PConfigMgr::getMetaConfig(ztdFilePath);
    
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
            // qDebug() << "[" << k << "] =";

            // grab array of values
            if (auto arr = value.as_array()) {
                for (const auto &item : *arr) {
                    // qDebug() << "\t" << QString::fromStdString(item.as_string()->get());
                }
            }

            continue;
        } else if (k == "dependencies") {
            // qDebug () << "[ Dependencies ] =";

            // parse dictionary and print key/value pairs
            if (auto depTable = value.as_table()) {
                if (depTable->empty()) {
                    // qDebug() << "\t\"\"";
                    continue;
                }
                for (const auto &[depKey, depValue] : *depTable) {
                    QString depK = QString::fromStdString(static_cast<std::string>(depKey));
                    if (auto depStr = depValue.as_string()) {
                        QString depV = QString::fromStdString(depStr->get());
                        // qDebug() << "\t" << depV;
                    }
                }
            }
            continue;
        }
        // qDebug() << "[" << k << "] = " << v;
    }

    config.clear();
}

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
