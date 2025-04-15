#include "PZooConfig.h"

PZooConfig::PZooConfig(QObject *parent) : QObject(parent) {

}

// for when user wants the default settings
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

    // [lib]
    m_res = "res0.dll";
    m_langLib = "lang0.dll";

    // [resource]
    m_path = "./dlupdate;./updates;./xpack2/loc;./xpack2;./dupdate;./zupdate1;./xpack1/loc;./xpack1;./zupdate;./loc;.";

    // [user]
    m_fullscreen = "0";
    m_screenwidth = "800";
    m_screenheight = "600";
    m_UpdateRate = "60";
    m_DrawRate = "60";
    m_lastfile = "";
    m_unlockCount = "0";
    m_unlockEntity = QStringList();
    // [advanced]
    m_drag = "3";
    m_click = "1";
    m_normal = "2";
    m_loadHalfAnims = "0";
    m_use8BitSound = "0";
    m_level = "2";
    m_devModeEnabled = "0";

    // [Map]
    m_mapX = "75";
    m_mapY = "75";

    // [UI]
    m_useAlternateCursors = "0";
    m_tooltipDelay = "1";
    m_tooltipDuration = "3000";
    m_MessageDisplay = "1";
    m_mouseScrollThreshold = "1";
    m_mouseScrollDelay = "1";
    m_mouseScrollX = "69";
    m_mouseScrollY = "69";
    m_keyScrollX = "64";
    m_keyScrollY = "64";
    m_minimumMessageInterval = "60";
    m_defaultEditCharLimit = "22";
    m_noMenuMusic = "0";
    m_menuMusic = "sounds/mainmenu.wav";
    m_menuMusicAttenuation = "-1000";
    m_userAttenuation = "0";
    m_helpType = "1";
    m_playMovie = "1";
    m_movievolume1 = "-1000";
    m_playSecondMovie = "1";
    m_movievolume2 = "-1000";
    m_maxShortTooltipWidth = "200";
    m_maxLongTooltipWidth = "400";
    m_progressLeft = "231";
    m_progressTop = "420";
    m_progressRight = "581";
    m_progressBottom = "435";
    m_progressRed = "255";
    m_progressGreen = "184";
    m_progressBlue = "43";
    m_progressShadowXOffset = "-3";
    m_progressShadowYOffset = "3";
    m_progressShadowRed = "32";
    m_progressShadowGreen = "32";
    m_progressShadowBlue = "32";
    m_completedExhibitAttenuation = "100";
    m_MSStartingCash = "75000";
    m_MSCashIncrement = "5000";
    m_MSMinCash = "10000";
    m_MSMaxCash = "500000";
    m_startedFirstTutorial = "1";
    m_progresscalls = "1280";
    m_startedDinoTutorial = "1";
    m_startedAquaTutorial = "1";
    m_lastWindowX = "522";
    m_lastWindowY = "86";

    // [scenario]
    m_tutorial = "0";
    m_ba = "0";
    m_bb = "0";
    m_bc = "0";
    m_bd = "0";
    m_be = "0";
    m_bf = "0";
    m_bg = "0";
    m_bh = "0";
    m_bi = "0";
    m_ca = "0";
    m_cb = "0";
    m_cc = "0";
    m_cd = "0";
    m_ce = "0";
    m_cf = "0";
    m_cg = "0";
    m_ch = "0";
    m_ci = "0";
    m_cj = "0";
    m_ck = "0";
    m_cl = "0";
    m_cm = "0";
    m_cn = "0";
    m_da = "0";
    m_db = "0";
    m_dc = "0";
    m_dd = "0";
    m_de = "0";
    m_df = "0";
    m_dg = "0";
    m_dh = "0";
    m_ea = "0";
    m_fa = "0";
    m_fb = "0";
    m_ga = "0";
    m_gb = "0";
    m_gc = "0";
    m_gd = "0";

    // [ai]
    m_showAIInfo = "0";
    m_showName = "0";
    m_showPosition = "0";
    m_showAnimalAIInfo = "0";
    m_showGuestAIInfo = "0";
    m_showStaffAIInfo = "0";
    m_showStatusVars = "0";
    m_showFunctionCall = "0";
    m_showEvents = "0";
    m_showBuildingAIInfo = "0";
    m_showSelected = "0";
    m_showFrame = "0";
    m_showGoal = "0";
}