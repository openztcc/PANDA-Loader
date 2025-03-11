#include <QTest/QTest>
#include "../PDatabaseMgr.h"

class PTestDatabaseMgr : public QObject
{
    Q_OBJECT
private slots:
    void testInsertMod_data();
    void testInsertMod();
    void testDeleteMod_data();
    void testDeleteMod();
    void testUpdateMod_data();
    void testUpdateMod();
    void testAddDependency_data();
    void testAddDependency();
};

QString testDataDir = QFINDTESTDATA("test_data/");

void PTestDatabaseMgr::testInsertMod_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("desc");
    QTest::addColumn<std::vector<QString>>("authors");
    QTest::addColumn<QString>("version");
    QTest::addColumn<QString>("path");
    QTest::addColumn<bool>("enabled");
    QTest::addColumn<std::vector<QString>>("tags");
    QTest::addColumn<QString>("modId");
    QTest::addColumn<std::vector<PDatabaseMgr::PDependency>>("dependencies");
    QTest::addColumn<bool>("expectedResult");

    // Test case 1: Insert valid mod
    std::vector<PDatabaseMgr::PDependency> deps;
    deps.push_back({"mod_id", "name", "min_version", false, "ordering", "link"});
    QTest::newRow("valid mod") << "mod_name" << "mod_desc" << std::vector<QString>{"author1", "author2"} << "1.0.0" << testDataDir + "valid.ztd" << true << std::vector<QString>{"tag1", "tag2"} << "mod_id" << deps << true;

    // Test case 2: Insert mod with no dependencies
    QTest::newRow("no dependencies") << "mod_name" << "mod_desc" << std::vector<QString>{"author1", "author2"} << "1.0.0" << testDataDir + "valid.ztd" << true << std::vector<QString>{"tag1", "tag2"} << "mod_id" << std::vector<PDatabaseMgr::PDependency>{} << true;

    // Test case 3: Insert mod with no tags
    QTest::newRow("no tags") << "mod_name" << "mod_desc" << std::vector<QString>{"author1", "author2"} << "1.0.0" << testDataDir + "valid.ztd" << true << std::vector<QString>{} << "mod_id" << deps << true;

    // Test case 4: Insert mod with no authors
    QTest::newRow("no authors") << "mod_name" << "mod_desc" << std::vector<QString>{} << "1.0.0" << testDataDir + "valid.ztd" << true << std::vector<QString>{"tag1", "tag2"} << "mod_id" << deps << true;

    // Test case 5: Insert mod with no tags, authors, or dependencies
    QTest::newRow("no tags, authors, or dependencies") << "mod_name" << "mod_desc" << std::vector<QString>{} << "1.0.0" << testDataDir + "valid.ztd" << true << std::vector<QString>{} << "mod_id" << std::vector<PDatabaseMgr::PDependency>{} << true;

    // Test case 6: Insert mod with no path
    QTest::newRow("no path") << "mod_name" << "mod_desc" << std::vector<QString>{"author1", "author2"} << "1.0.0" << "" << true << std::vector<QString>{"tag1", "tag2"} << "mod_id" << deps << false;

    // Test case 7: Insert mod with no version
    QTest::newRow("no version") << "mod_name" << "mod_desc" << std::vector<QString>{"author1", "author2"} << "" << testDataDir + "valid.ztd" << true << std::vector<QString>{"tag1", "tag2"} << "mod_id" << deps << false;

    // Test case 8: Insert mod with no name
    QTest::newRow("no name") << "" << "mod_desc" << std::vector<QString>{"author1", "author2"} << "1.0.0" << testDataDir + "valid.ztd" << true << std::vector<QString>{"tag1", "tag2"} << "mod_id" << deps << false;

    // Test case 9: Insert mod with no modId
    QTest::newRow("no modId") << "mod_name" << "mod_desc" << std::vector<QString>{"author1", "author2"} << "1.0.0" << testDataDir + "valid.ztd" << true << std::vector<QString>{"tag1", "tag2"} << "" << deps << false;

    // Test case 10: Insert mod with no description
    QTest::newRow("no description") << "mod_name" << "" << std::vector<QString>{"author1", "author2"} << "1.0.0" << testDataDir + "valid.ztd" << true << std::vector<QString>{"tag1", "tag2"} << "mod_id" << deps << false;
}

void PTestDatabaseMgr::testInsertMod()
{
    QFETCH(QString, name);
    QFETCH(QString, desc);
    QFETCH(std::vector<QString>, authors);
    QFETCH(QString, version);
    QFETCH(QString, path);
    QFETCH(bool, enabled);
    QFETCH(std::vector<QString>, tags);
    QFETCH(QString, modId);
    QFETCH(std::vector<PDatabaseMgr::PDependency>, dependencies);
    QFETCH(bool, expectedResult);

    PDatabaseMgr dbMgr;
    QVERIFY(dbMgr.openDatabase());
    QVERIFY(dbMgr.createTables());

    bool result = dbMgr.insertMod(name, desc, authors, version, path, enabled, tags, modId, dependencies);
    QCOMPARE(result, expectedResult);

    dbMgr.closeDatabase();
}

void PTestDatabaseMgr::testDeleteMod_data()
{
    QTest::addColumn<QString>("modId");
    QTest::addColumn<bool>("expectedResult");

    // Test case 1: Delete valid mod
    QTest::newRow("valid mod") << "mod_id" << true;

    // Test case 2: Delete non-existent mod
    QTest::newRow("non-existent mod") << "non-existent" << false;
}

void PTestDatabaseMgr::testDeleteMod()
{
    QFETCH(QString, modId);
    QFETCH(bool, expectedResult);

    PDatabaseMgr dbMgr;
    QVERIFY(dbMgr.openDatabase());
    QVERIFY(dbMgr.createTables());

    QVERIFY(dbMgr.insertMod("mod_name", "mod_desc", std::vector<QString>{"author1", "author2"}, "1.0.0", testDataDir + "valid.ztd", true, std::vector<QString>{"tag1", "tag2"}, "mod_id", std::vector<PDatabaseMgr::PDependency>{}));

    bool result = dbMgr.deleteMod(modId);
    QCOMPARE(result, expectedResult);

    dbMgr.closeDatabase();
}

void PTestDatabaseMgr::testUpdateMod_data()
{
    QTest::addColumn<QString>("modId");
    QTest::addColumn<QString>("key");
    QTest::addColumn<QString>("value");
    QTest::addColumn<bool>("expectedResult");

    // Test case 1: Update valid mod
    QTest::newRow("valid mod") << "mod_name" << "title" << "new_title" << true;

    // Test case 2: Update non-existent mod
    QTest::newRow("non-existent mod") << "non-existent" << "title" << "new_title" << false;
}

void PTestDatabaseMgr::testUpdateMod()
{
    QFETCH(QString, modId);
    QFETCH(QString, key);
    QFETCH(QString, value);
    QFETCH(bool, expectedResult);

    PDatabaseMgr dbMgr;
    QVERIFY(dbMgr.openDatabase());
    QVERIFY(dbMgr.createTables());

    QVERIFY(dbMgr.insertMod("mod_name", "mod_desc", std::vector<QString>{"author1", "author2"}, "1.0.0", testDataDir + "valid.ztd", true, std::vector<QString>{"tag1", "tag2"}, "mod_id", std::vector<PDatabaseMgr::PDependency>{}));

    bool result = dbMgr.updateMod(modId, key, value);
    QCOMPARE(result, expectedResult);

    dbMgr.closeDatabase();
}

void PTestDatabaseMgr::testAddDependency_data()
{
    QTest::addColumn<QString>("modId");
    QTest::addColumn<PDatabaseMgr::PDependency>("dependency");
    QTest::addColumn<bool>("expectedResult");

    // Test case 1: Add valid dependency
    QTest::newRow("valid dependency") << "mod_id" << PDatabaseMgr::PDependency{"mod_id", "name", "min_version", false, "ordering", "link"} << true;

    // Test case 2: Add dependency with no modId
    QTest::newRow("no modId") << "" << PDatabaseMgr::PDependency{"mod_id", "name", "min_version", false, "ordering", "link"} << false;

    // Test case 3: Add dependency with no name
    QTest::newRow("no name") << "mod_id" << PDatabaseMgr::PDependency{"mod_id", "", "min_version", false, "ordering", "link"} << false;

    // Test case 4: Add dependency with no min_version
    QTest::newRow("no min_version") << "mod_id" << PDatabaseMgr::PDependency{"mod_id", "name", "", false, "ordering", "link"} << false;

    // Test case 5: Add dependency with no ordering
    QTest::newRow("no ordering") << "mod_id" << PDatabaseMgr::PDependency{"mod_id", "name", "min_version", false, "", "link"} << false;

    // Test case 6: Add dependency with no link
    QTest::newRow("no link") << "mod_id" << PDatabaseMgr::PDependency{"mod_id", "name", "min_version", false, "ordering", ""} << false;
}

void PTestDatabaseMgr::testAddDependency()
{
    QFETCH(QString, modId);
    QFETCH(PDatabaseMgr::PDependency, dependency);
    QFETCH(bool, expectedResult);

    PDatabaseMgr dbMgr;
    QVERIFY(dbMgr.openDatabase());
    QVERIFY(dbMgr.createTables());

    QVERIFY(dbMgr.insertMod("mod_name", "mod_desc", std::vector<QString>{"author1", "author2"}, "1.0.0", testDataDir + "valid.ztd", true, std::vector<QString>{"tag1", "tag2"}, "mod_id", std::vector<PDatabaseMgr::PDependency>{}));

    bool result = dbMgr.addDependency(modId, dependency);
    QCOMPARE(result, expectedResult);

    dbMgr.closeDatabase();
}

QTEST_MAIN(PTestDatabaseMgr)
#include "PTestDatabaseMgr.moc"
