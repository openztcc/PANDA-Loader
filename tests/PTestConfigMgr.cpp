#include <QtTest/QtTest>
#include "../src/PConfigMgr.h"

class PTestConfigMgr : public QObject
{
    Q_OBJECT
private slots:
    void testGetMetaConfig_data();
    void testGetMetaConfig();
    void testUpdateMetaConfig_data();
    void testUpdateMetaConfig();
    void testRemoveMetaConfig_data();
    void testRemoveMetaConfig();
    // void testAddMetaConfig_data();
    // void testAddMetaConfig();
    // void testGetZooIniConfig_data();
    // void testGetZooIniConfig();
    // void testUpdateZooIniConfig_data();
    // void testUpdateZooIniConfig();
    // void testRemoveZooIniConfig_data();
    // void testRemoveZooIniConfig();
    void testGetAllConfigInZtd_data();
    void testGetAllConfigInZtd();
    void testGetCoreConfigInZtd_data();
    void testGetCoreConfigInZtd();
    void testGetIconAniPaths_data();
    void testGetIconAniPaths();
    void testGetIconAniConfigInZtd_data();
    void testGetIconAniConfigInZtd();
    void testGetIconPaths_data();
    void testGetIconPaths();
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

void PTestConfigMgr::testUpdateMetaConfig_data()
{
    QTest::addColumn<QString>("ztdFilePath");
    QTest::addColumn<toml::table>("config");
    QTest::addColumn<bool>("expected");

    // Valid TOML table
    toml::table validConfig;
    validConfig.insert_or_assign("authors", toml::array{"Goosifer", "Finn"});
    validConfig.insert_or_assign("tags", toml::array{"tag1", "tag2"});
    validConfig.insert_or_assign("dependencies", toml::table{{"dep1", "1.0.0"}, {"dep2", "2.0.0"}});

    // Invalid TOML table (wrong types)
    toml::table invalidConfig;
    invalidConfig.insert_or_assign("authors", "Not an array");  // Wrong type (should be array)
    invalidConfig.insert_or_assign("tags", toml::array{"tag1", 123}); // Mixed types
    invalidConfig.insert_or_assign("dependencies", toml::table{{"dep1", 1.0}}); // Wrong type (should be string)

    QTest::newRow("valid meta.toml") << testDataDir + "config_update_valid.ztd" << validConfig << true;
    QTest::newRow("invalid meta.toml") << testDataDir + "config_update_invalid.ztd" << invalidConfig << false;

    validConfig.clear();
    invalidConfig.clear();
}

void PTestConfigMgr::testUpdateMetaConfig()
{
    QFETCH(QString, ztdFilePath);
    QFETCH(toml::table, config);
    QFETCH(bool, expected);

    // Perform update
    bool result = PConfigMgr::updateMetaConfig(ztdFilePath, config);
    QVERIFY(result == expected);

    QString correctMeta = R"(  
        [authors]
        authors = ["Goosifer", "Finn"]
        tags = ["tag1", "tag2"]
        
        [dependencies]
        dep1 = "1.0.0"
        dep2 = "2.0.0"
    )";

    // Did it update correctly?
    if (expected) {
        try {
            toml::table newConfig = PConfigMgr::getMetaConfig(ztdFilePath);
            toml::table correctConfig = toml::parse(correctMeta.toStdString());

            // Compare the two tables
            bool metaTest = false;
            QString authorValue = PConfigMgr::getKeyValue("authors", newConfig);
            QString tagValue = PConfigMgr::getKeyValue("tags", newConfig);
            QString depValue = PConfigMgr::getKeyValue("dependencies", newConfig);

            // Expected values
            QString correctAuthorValue = PConfigMgr::getKeyValue("authors", correctConfig);
            QString correctTagValue = PConfigMgr::getKeyValue("tags", correctConfig);
            QString correctDepValue = PConfigMgr::getKeyValue("dependencies", correctConfig);

            // Check if matching
            if (authorValue == correctAuthorValue && tagValue == correctTagValue && depValue == correctDepValue) {
                metaTest = true;
            }

            QVERIFY(metaTest);
        } catch (const toml::parse_error &e) {
            QFAIL(("TOML file parsing failed after update: " + std::string(e.what())).c_str());
        }
    }

    // Clean up
    config.clear();
}

void PTestConfigMgr::testRemoveMetaConfig_data()
{
    QTest::addColumn<QString>("ztdFilePath");
    QTest::addColumn<bool>("expected");

    QTest::newRow("remove meta.toml") << testDataDir + "config_remove_valid.ztd" << true;
    QTest::newRow("remove non-existent meta.toml") << testDataDir + "config_remove_invalid.ztd" << false;
}

void PTestConfigMgr::testRemoveMetaConfig()
{
    QFETCH(QString, ztdFilePath);
    QFETCH(bool, expected);

    // Perform removal
    bool result = PConfigMgr::removeMetaConfig(ztdFilePath);
    QVERIFY(result == expected);

    // Check if it was removed
    if (expected) {
        toml::table config = PConfigMgr::getMetaConfig(ztdFilePath);
        QVERIFY(config.empty());
    }
}

void PTestConfigMgr::testGetAllConfigInZtd_data()
{
    QTest::addColumn<QString>("ztdFilePath");
    QTest::addColumn<bool>("expected");

    QTest::newRow("valid ucb ztd") << testDataDir + "getfile_valid.ztd" << true;
    QTest::newRow("invalid ztd") << testDataDir + "config_invalid.ztd" << false;
}

void PTestConfigMgr::testGetAllConfigInZtd()
{
    QFETCH(QString, ztdFilePath);
    QFETCH(bool, expected);

    // Get all config files in ztd
    std::vector<std::unique_ptr<PConfigMgr::IniData>> configFiles = PConfigMgr::getAllConfigInZtd(ztdFilePath);

    qDebug() << "Config files found:" << configFiles.size();
    for (const auto &file : configFiles) {
        qDebug() << "File:" << file->filename << file->path;
        if (file->settings) {
            qDebug() << "Settings:" << file->settings->fileName();
        } else {
            qDebug() << "Settings: nullptr";
        }
    }

    if (expected) {
        QVERIFY(!configFiles.empty());
        for (const auto &file : configFiles) {
            QVERIFY(file->settings != nullptr);
        }
    } else {
        QVERIFY(configFiles.empty());
    }
}

void PTestConfigMgr::testGetCoreConfigInZtd_data()
{
    QTest::addColumn<QString>("ztdFilePath");
    QTest::addColumn<bool>("expected");

    QTest::newRow("valid ucb ztd") << testDataDir + "getfile_valid.ztd" << true;
    QTest::newRow("valid uca ztd") << testDataDir + "getfile_valid_uca.ztd" << true;
    QTest::newRow("invalid ztd") << testDataDir + "config_invalid.ztd" << false;
}

void PTestConfigMgr::testGetCoreConfigInZtd()
{
    QFETCH(QString, ztdFilePath);
    QFETCH(bool, expected);

    // Get core config files in ztd
    std::vector<std::unique_ptr<PConfigMgr::IniData>> coreConfigFiles = PConfigMgr::getCoreConfigInZtd(ztdFilePath);

    qDebug() << "Core config files found:" << coreConfigFiles.size();
    for (const auto &file : coreConfigFiles) {
        qDebug() << "File:" << file->filename << file->path;
        if (file->settings) {
            qDebug() << "Settings:" << file->settings->fileName();
        } else {
            qDebug() << "Settings: nullptr";
        }
    }

    if (expected) {
        QVERIFY(!coreConfigFiles.empty());
        for (const auto &file : coreConfigFiles) {
            QVERIFY(file->settings != nullptr);
        }
    } else {
        QVERIFY(coreConfigFiles.empty());
    }
}

void PTestConfigMgr::testGetIconAniPaths_data()
{
    QTest::addColumn<QString>("ztdFilePath");
    QTest::addColumn<bool>("expected");

    QTest::newRow("ucb ztd") << testDataDir + "getfile_valid.ztd" << true;
    QTest::newRow("uca ztd") << testDataDir + "getfile_valid_uca.ztd" << true;
    QTest::newRow("invalid ztd") << testDataDir + "config_invalid.ztd" << false;
}

void PTestConfigMgr::testGetIconAniPaths()
{
    QFETCH(QString, ztdFilePath);
    QFETCH(bool, expected);

    // Get icon animation paths
    QStringList iconAniPaths = PConfigMgr::getIconAniPaths(ztdFilePath);

    qDebug() << "Icon animation paths found:" << iconAniPaths.size();
    for (const auto &path : iconAniPaths) {
        qDebug() << "Path:" << path;
    }

    if (expected) {
        QVERIFY(!iconAniPaths.empty());
    } else {
        QVERIFY(iconAniPaths.empty());
    }
}

void PTestConfigMgr::testGetIconAniConfigInZtd_data()
{
    QTest::addColumn<QString>("ztdFilePath");
    QTest::addColumn<bool>("expected");

    QTest::newRow("valid ucb ztd") << testDataDir + "getfile_valid.ztd" << true;
    QTest::newRow("valid uca ztd") << testDataDir + "getfile_valid_uca.ztd" << true;
    QTest::newRow("invalid ztd") << testDataDir + "config_invalid.ztd" << false;
}

void PTestConfigMgr::testGetIconAniConfigInZtd()
{
    QFETCH(QString, ztdFilePath);
    QFETCH(bool, expected);

    // Get icon animation config files in ztd
    std::vector<std::unique_ptr<PConfigMgr::IniData>> iconAniConfigFiles = PConfigMgr::getIconAniConfigInZtd(ztdFilePath);

    qDebug() << "Icon animation config files found:" << iconAniConfigFiles.size();
    for (const auto &file : iconAniConfigFiles) {
        qDebug() << "File:" << file->filename << file->path;
        if (file->settings) {
            qDebug() << "Settings:" << file->settings->fileName();
        } else {
            qDebug() << "Settings: nullptr";
        }
    }

    if (expected) {
        QVERIFY(!iconAniConfigFiles.empty());
        for (const auto &file : iconAniConfigFiles) {
            QVERIFY(file->settings != nullptr);
        }
    } else {
        QVERIFY(iconAniConfigFiles.empty());
    }
}

void PTestConfigMgr::testGetIconPaths_data()
{
    QTest::addColumn<QString>("ztdFilePath");
    QTest::addColumn<bool>("expected");

    QTest::newRow("ucb ztd") << testDataDir + "getfile_valid.ztd" << true;
    QTest::newRow("uca ztd") << testDataDir + "getfile_valid_uca.ztd" << true;
    QTest::newRow("invalid ztd") << testDataDir + "config_invalid.ztd" << false;
}

void PTestConfigMgr::testGetIconPaths()
{
    QFETCH(QString, ztdFilePath);
    QFETCH(bool, expected);

    // Get icon paths
    QStringList iconPaths = PConfigMgr::getIconPaths(ztdFilePath);

    qDebug() << "Icon paths found:" << iconPaths.size();
    for (const auto &path : iconPaths) {
        qDebug() << "Path:" << path;
    }

    if (expected) {
        QVERIFY(!iconPaths.empty());
    } else {
        QVERIFY(iconPaths.empty());
    }
}

QTEST_MAIN(PTestConfigMgr)
#include "PTestConfigMgr.moc"
