#include <QtTest/QtTest>
#include <QVector>
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
    QTest::addColumn<QVector<QString>>("authors");
    QTest::addColumn<QString>("version");
    QTest::addColumn<QString>("path");
    QTest::addColumn<bool>("enabled");
    QTest::addColumn<QVector<QString>>("tags");
    QTest::addColumn<QVector<PDatabaseMgr::PDependency>>("dependencies");
    QTest::addColumn<bool>("expectedResult");

    QVector<PDatabaseMgr::PDependency> deps;
    deps.push_back({"mod_id", "name", "min_version", false, "ordering", "link"});

    // valid case
    QTest::newRow("valid mod") 
        << "mod_name" << "mod_desc" 
        << QVector<QString>{"author1", "author2"} 
        << "1.0.0" << testDataDir + "valid.ztd" 
        << true << QVector<QString>{"tag1", "tag2"} 
        << deps << true;

    // valid case with optional fields empty
    QTest::newRow("valid mod (no authors/tags/dependencies)") 
        << "mod_name" << "mod_desc" 
        << QVector<QString>{} 
        << "1.0.0" << testDataDir + "valid.ztd" 
        << true << QVector<QString>{} 
        << QVector<PDatabaseMgr::PDependency>{} << true;

    // missing required fields (name, version, path, modId)
    QTest::newRow("missing name") 
        << "" << "mod_desc" 
        << QVector<QString>{"author1", "author2"} 
        << "1.0.0" << testDataDir + "valid.ztd" 
        << true << QVector<QString>{"tag1", "tag2"} 
        << deps << false;

    QTest::newRow("missing version") 
        << "mod_name" << "mod_desc" 
        << QVector<QString>{"author1", "author2"} 
        << "" << testDataDir + "valid.ztd" 
        << true << QVector<QString>{"tag1", "tag2"} 
        << deps << false;

    QTest::newRow("missing path") 
        << "mod_name" << "mod_desc" 
        << QVector<QString>{"author1", "author2"} 
        << "1.0.0" << "" 
        << true << QVector<QString>{"tag1", "tag2"} 
        << deps << false;
}

void PTestDatabaseMgr::testInsertMod()
{
    QFETCH(QString, name);
    QFETCH(QString, desc);
    QFETCH(QVector<QString>, authors);
    QFETCH(QString, version);
    QFETCH(QString, path);
    QFETCH(bool, enabled);
    QFETCH(QVector<QString>, tags);
    QFETCH(QVector<PDatabaseMgr::PDependency>, dependencies);
    QFETCH(bool, expectedResult);

    // unique modId for each test case
    QString uniqueModId = QString("mod_id_%1").arg(QUuid::createUuid().toString(QUuid::WithoutBraces));

    PDatabaseMgr dbMgr;
    QVERIFY(dbMgr.openDatabase());

    bool result = dbMgr.insertMod(name, desc, authors, version, path, enabled, tags, uniqueModId, dependencies);
    QCOMPARE(result, expectedResult);

    // Clean up
    dbMgr.deleteMod(uniqueModId);

    dbMgr.closeDatabase();
}


void PTestDatabaseMgr::testDeleteMod_data()
{
    QTest::addColumn<QString>("modId");
    QTest::addColumn<bool>("expectedResult");

    // Test case 1: Delete valid mod
    QTest::newRow("valid mod") << "delete.this" << true;

    // Test case 2: Delete non-existent mod
    QTest::newRow("non-existent mod") << "mod.does.not.exist" << false;
}

void PTestDatabaseMgr::testDeleteMod()
{
    QFETCH(QString, modId);
    QFETCH(bool, expectedResult);

    PDatabaseMgr dbMgr;
    QVERIFY(dbMgr.openDatabase());

    // Clean up in case of previous runs
    dbMgr.deleteMod(modId); 

    QVERIFY(dbMgr.insertMod("mod_name", "mod_desc", QVector<QString>{"author1", "author2"}, "1.0.0", testDataDir + "valid.ztd", true, QVector<QString>{"tag1", "tag2"}, "delete.this", QVector<PDatabaseMgr::PDependency>{}));

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
    QTest::newRow("valid mod") << "update.mod_id" << "title" << "new_title" << true;

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

    QVERIFY(dbMgr.insertMod("mod_name", "mod_desc", QVector<QString>{"author1", "author2"}, "1.0.0", testDataDir + "valid.ztd", true, QVector<QString>{"tag1", "tag2"}, "update.mod_id", QVector<PDatabaseMgr::PDependency>{}));

    bool result = dbMgr.updateMod(modId, key, value);
    QCOMPARE(result, expectedResult);

    // Clean up
    dbMgr.deleteMod(modId);

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

    QVERIFY(dbMgr.insertMod("mod_name", "mod_desc", QVector<QString>{"author1", "author2"}, "1.0.0", testDataDir + "valid.ztd", true, QVector<QString>{"tag1", "tag2"}, "mod_id", QVector<PDatabaseMgr::PDependency>{}));

    bool result = dbMgr.addDependency(modId, dependency);
    QCOMPARE(result, expectedResult);

    dbMgr.closeDatabase();
}

QTEST_MAIN(PTestDatabaseMgr)
#include "PTestDatabaseMgr.moc"
