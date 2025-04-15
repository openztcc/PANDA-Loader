#include "PZooConfig.h"

PZooConfig::PZooConfig(QObject *parent) : QObject(parent) {

}

void PZooConfig::resetZooConfig() {
    // [debug]
    m_logCutoff = "15";
    m_sendLogfile = "0";
    m_sendDebugger = "1";
    m_deltaLog1 = "0";
    m_deltaLog0 = "0";
    m_drawfps = "0";
    m_drawfpsx = "720";
    m_drawfpsy = "20";

    // [mgr]
    m_aimgr = "ZTAIMgr";
    m_worldmgr = "ZTWorldMgr";
    m_gamemgr = "BFGameMgr";
    m_scenariomgr = "ZTScenarioMgr";
    m_scriptmgr = "BFScriptMgr";
    m_soundmgr = "BFSoundMgr";
    m_terrainmgr = "ZTAdvTerrainMgr";
    // [language]
    m_lang = "9";
    m_sublang = "1";

    // [ztd]
    
}