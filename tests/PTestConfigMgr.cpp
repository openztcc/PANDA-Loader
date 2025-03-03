#include <QtTest/QtTest>
#include "../PConfigMgr.h"

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
    void testAddMetaConfig_data();
    void testAddMetaConfig();
    void testGetZooIniConfig_data();
    void testGetZooIniConfig();
    void testUpdateZooIniConfig_data();
    void testUpdateZooIniConfig();
    void testRemoveZooIniConfig_data();
    void testRemoveZooIniConfig();
};

// Statics
QString testDataDir = QFINDTESTDATA("test_data/");



QTEST_MAIN(PTestConfigMgr)
#include "PTestConfigMgr.moc"
