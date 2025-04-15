#include "PZooConfig.h"

PZooConfig::PZooConfig(QObject *parent) : QObject(parent), m_zooConfigPath("") {
    if (m_zooConfigPath.isEmpty()) {
        m_zooConfigPath = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\zoo.ini";
    }

    // Initialize the config table with default values
    m_configTable = {
        {"debug", {}}, {"mgr", {}}, {"language", {}}, {"lib", {}}, {"resource", {}}, {"user", {}}, {"advanced", {}}, {"Map", {}}, {"UI", {}}, {"scenario", {}}, {"ai", {}},
    };
    m_dirty = false;
}

// for when user wants the default settings
PZooConfig PZooConfig::defaultConfig() {
    PZooConfig config;
    
    // [debug]
    config.m_logCutoff = "15";
    config.m_sendLogfile = "0";
    config.m_sendDebugger = "1";
    config.m_deltaLog1 = "0";
    config.m_deltaLog0 = "0";
    config.m_drawfps = "0";
    config.m_drawfpsx = "720";
    config.m_drawfpsy = "20";

    // [mgr]
    config.m_aimgr = "ZTAIMgr";
    config.m_worldmgr = "ZTWorldMgr";
    config.m_gamemgr = "BFGameMgr";
    config.m_scenariomgr = "ZTScenarioMgr";
    config.m_scriptmgr = "BFScriptMgr";
    config.m_soundmgr = "BFSoundMgr";
    config.m_terrainmgr = "ZTAdvTerrainMgr";
    
    // [language]
    config.m_lang = "9";
    config.m_sublang = "1";

    // [lib]
    config.m_res = "res0.dll";
    config.m_langLib = "lang0.dll";

    // [resource]
    config.m_path = QStringList() << "./dlupdate" << "./updates" << "./xpack2/loc" << "./xpack2" << "./dupdate" << "./zupdate1" << "./xpack1/loc" << "./xpack1" << "./zupdate" << "./loc" << ".";

    // [user]
    config.m_fullscreen = "0";
    config.m_screenwidth = "800";
    config.m_screenheight = "600";
    config.m_UpdateRate = "60";
    config.m_DrawRate = "60";
    config.m_lastfile = "";
    config.m_unlockCount = "0"; // not in ini by default
    config.m_unlockEntity = QStringList(); // not in ini by default
    
    // [advanced]
    config.m_drag = "3";
    config.m_click = "1";
    config.m_normal = "2";
    config.m_loadHalfAnims = "0";
    config.m_use8BitSound = "0";
    config.m_level = "2";
    config.m_devModeEnabled = "0";

    // [Map]
    config.m_mapX = "75";
    config.m_mapY = "75";

    // [UI]
    config.m_useAlternateCursors = "0";
    config.m_tooltipDelay = "1";
    config.m_tooltipDuration = "3000";
    config.m_MessageDisplay = "1";
    config.m_mouseScrollThreshold = "1";
    config.m_mouseScrollDelay = "1";
    config.m_mouseScrollX = "69";
    config.m_mouseScrollY = "69";
    config.m_keyScrollX = "64";
    config.m_keyScrollY = "64";
    config.m_minimumMessageInterval = "60";
    config.m_defaultEditCharLimit = "22";
    config.m_noMenuMusic = "0";
    config.m_menuMusic = "sounds/mainmenu.wav";
    config.m_menuMusicAttenuation = "-1000";
    config.m_userAttenuation = "0";
    config.m_helpType = "1";
    config.m_playMovie = "1";
    config.m_movievolume1 = "-1000";
    config.m_playSecondMovie = "1";
    config.m_movievolume2 = "-1000";
    config.m_maxShortTooltipWidth = "200";
    config.m_maxLongTooltipWidth = "400";
    config.m_progressLeft = "231";
    config.m_progressTop = "420";
    config.m_progressRight = "581";
    config.m_progressBottom = "435";
    config.m_progressRed = "255";
    config.m_progressGreen = "184";
    config.m_progressBlue = "43";
    config.m_progressShadowXOffset = "-3";
    config.m_progressShadowYOffset = "3";
    config.m_progressShadowRed = "32";
    config.m_progressShadowGreen = "32";
    config.m_progressShadowBlue = "32";
    config.m_completedExhibitAttenuation = "100";
    config.m_MSStartingCash = "75000";
    config.m_MSCashIncrement = "5000";
    config.m_MSMinCash = "10000";
    config.m_MSMaxCash = "500000";
    config.m_startedFirstTutorial = "1";
    config.m_progresscalls = "1280";
    config.m_startedDinoTutorial = "1";
    config.m_startedAquaTutorial = "1";
    config.m_lastWindowX = "522";
    config.m_lastWindowY = "86";

    // [scenario]
    config.m_tutorial = "0";
    config.m_aa = "0";
    config.m_ab = "0";
    config.m_ac = "0";
    config.m_ad = "0";
    config.m_ae = "0";
    config.m_af = "0";
    config.m_ag = "0";
    config.m_ba = "0";
    config.m_bb = "0";
    config.m_bc = "0";
    config.m_bd = "0";
    config.m_be = "0";
    config.m_bf = "0";
    config.m_bg = "0";
    config.m_bh = "0";
    config.m_bi = "0";
    config.m_ca = "0";
    config.m_cb = "0";
    config.m_cc = "0";
    config.m_cd = "0";
    config.m_ce = "0";
    config.m_cf = "0";
    config.m_cg = "0";
    config.m_ch = "0";
    config.m_ci = "0";
    config.m_cj = "0";
    config.m_ck = "0";
    config.m_cl = "0";
    config.m_cm = "0";
    config.m_cn = "0";
    config.m_da = "0";
    config.m_db = "0";
    config.m_dc = "0";
    config.m_dd = "0";
    config.m_de = "0";
    config.m_df = "0";
    config.m_dg = "0";
    config.m_dh = "0";
    config.m_ea = "0";
    config.m_fa = "0";
    config.m_fb = "0";
    config.m_ga = "0";
    config.m_gb = "0";
    config.m_gc = "0";
    config.m_gd = "0";

    // [ai]
    config.m_showAIInfo = "0";
    config.m_showName = "0";
    config.m_showPosition = "0";
    config.m_showAnimalAIInfo = "0";
    config.m_showGuestAIInfo = "0";
    config.m_showStaffAIInfo = "0";
    config.m_showStatusVars = "0";
    config.m_showFunctionCall = "0";
    config.m_showEvents = "0";
    config.m_showBuildingAIInfo = "0";
    config.m_showSelected = "0";
    config.m_showFrame = "0";
    config.m_showGoal = "0";

    return config;
}

void PZooConfig::updateTable(const QString &section, const QString &key, const QString &value) {
    m_configTable[section][key] = value.toStdString();
    emit configUpdated(section, key, value);
}

void PZooConfig::updateUnlockEntity(const QString &key, const QString &value) {
    m_unlockEntity.append(value);
    m_configTable["user"][key] = value.toStdString();
    emit unlockEntityUpdated(key, value);
}

// setter definitions

void PZooConfig::setZooConfigPath(const QString &path) {
    if (m_zooConfigPath != path) {
        m_zooConfigPath = path;
        emit zooConfigPathChanged(m_zooConfigPath);
    }
}


