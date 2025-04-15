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

void PZooConfig::setLogCutoff(const QString &logCutoff) {
    if (m_logCutoff != logCutoff) {
        m_logCutoff = logCutoff;
        emit logCutoffChanged(m_logCutoff);

        m_dirty = true;
        m_configTable["debug"]["logCutoff"] = logCutoff.toStdString();
    }
}

void PZooConfig::setSendLogfile(const QString &sendLogfile) {
    if (m_sendLogfile != sendLogfile) {
        m_sendLogfile = sendLogfile;
        emit sendLogfileChanged(m_sendLogfile);

        m_dirty = true;
        m_configTable["debug"]["sendLogfile"] = sendLogfile.toStdString();
    }
}

void PZooConfig::setSendDebugger(const QString &sendDebugger) {
    if (m_sendDebugger != sendDebugger) {
        m_sendDebugger = sendDebugger;
        emit sendDebuggerChanged(m_sendDebugger);

        m_dirty = true;
        m_configTable["debug"]["sendDebugger"] = sendDebugger.toStdString();
    }
}

void PZooConfig::setDeltaLog1(const QString &deltaLog1) {
    if (m_deltaLog1 != deltaLog1) {
        m_deltaLog1 = deltaLog1;
        emit deltaLog1Changed(m_deltaLog1);

        m_dirty = true;
        m_configTable["debug"]["deltaLog1"] = deltaLog1.toStdString();
    }
}

void PZooConfig::setDeltaLog0(const QString &deltaLog0) {
    if (m_deltaLog0 != deltaLog0) {
        m_deltaLog0 = deltaLog0;
        emit deltaLog0Changed(m_deltaLog0);

        m_dirty = true;
        m_configTable["debug"]["deltaLog0"] = deltaLog0.toStdString();
    }
}

void PZooConfig::setDrawfps(const QString &drawfps) {
    if (m_drawfps != drawfps) {
        m_drawfps = drawfps;
        emit drawfpsChanged(m_drawfps);

        m_dirty = true;
        m_configTable["debug"]["drawfps"] = drawfps.toStdString();
    }
}

void PZooConfig::setDrawfpsx(const QString &drawfpsx) {
    if (m_drawfpsx != drawfpsx) {
        m_drawfpsx = drawfpsx;
        emit drawfpsxChanged(m_drawfpsx);

        m_dirty = true;
        m_configTable["debug"]["drawfpsx"] = drawfpsx.toStdString();
    }
}

void PZooConfig::setDrawfpsy(const QString &drawfpsy) {
    if (m_drawfpsy != drawfpsy) {
        m_drawfpsy = drawfpsy;
        emit drawfpsyChanged(m_drawfpsy);

        m_dirty = true;
        m_configTable["debug"]["drawfpsy"] = drawfpsy.toStdString();
    }
}

void PZooConfig::setAimgr(const QString &aimgr) {
    if (m_aimgr != aimgr) {
        m_aimgr = aimgr;
        emit aimgrChanged(m_aimgr);

        m_dirty = true;
        m_configTable["mgr"]["aimgr"] = aimgr.toStdString();
    }
}

void PZooConfig::setWorldmgr(const QString &worldmgr) {
    if (m_worldmgr != worldmgr) {
        m_worldmgr = worldmgr;
        emit worldmgrChanged(m_worldmgr);

        m_dirty = true;
        m_configTable["mgr"]["worldmgr"] = worldmgr.toStdString();
    }
}

void PZooConfig::setGamemgr(const QString &gamemgr) {
    if (m_gamemgr != gamemgr) {
        m_gamemgr = gamemgr;
        emit gamemgrChanged(m_gamemgr);

        m_dirty = true;
        m_configTable["mgr"]["gamemgr"] = gamemgr.toStdString();
    }
}

void PZooConfig::setScenariomgr(const QString &scenariomgr) {
    if (m_scenariomgr != scenariomgr) {
        m_scenariomgr = scenariomgr;
        emit scenariomgrChanged(m_scenariomgr);

        m_dirty = true;
        m_configTable["mgr"]["scenariomgr"] = scenariomgr.toStdString();
    }
}

void PZooConfig::setScriptmgr(const QString &scriptmgr) {
    if (m_scriptmgr != scriptmgr) {
        m_scriptmgr = scriptmgr;
        emit scriptmgrChanged(m_scriptmgr);

        m_dirty = true;
        m_configTable["mgr"]["scriptmgr"] = scriptmgr.toStdString();
    }
}

void PZooConfig::setSoundmgr(const QString &soundmgr) {
    if (m_soundmgr != soundmgr) {
        m_soundmgr = soundmgr;
        emit soundmgrChanged(m_soundmgr);

        m_dirty = true;
        m_configTable["mgr"]["soundmgr"] = soundmgr.toStdString();
    }
}

void PZooConfig::setTerrainmgr(const QString &terrainmgr) {
    if (m_terrainmgr != terrainmgr) {
        m_terrainmgr = terrainmgr;
        emit terrainmgrChanged(m_terrainmgr);

        m_dirty = true;
        m_configTable["mgr"]["terrainmgr"] = terrainmgr.toStdString();
    }
}

void PZooConfig::setLang(const QString &lang) {
    if (m_lang != lang) {
        m_lang = lang;
        emit langChanged(m_lang);

        m_dirty = true;
        m_configTable["language"]["lang"] = lang.toStdString();
    }
}


void PZooConfig::setSublang(const QString &sublang) {
    if (m_sublang != sublang) {
        m_sublang = sublang;
        emit sublangChanged(m_sublang);

        m_dirty = true;
        m_configTable["language"]["sublang"] = sublang.toStdString();
    }
}

void PZooConfig::setRes(const QString &res) {
    if (m_res != res) {
        m_res = res;
        emit resChanged(m_res);

        m_dirty = true;
        m_configTable["lib"]["res"] = res.toStdString();
    }
}

void PZooConfig::setLangLib(const QString &langLib) {
    if (m_langLib != langLib) {
        m_langLib = langLib;
        emit langLibChanged(m_langLib);

        m_dirty = true;
        m_configTable["lib"]["lang"] = langLib.toStdString();
    }
}

void PZooConfig::setPath(const QStringList &path) {
    if (m_path != path) {
        m_path = path;
        emit pathChanged(m_path);

        m_dirty = true;
        m_configTable["resource"]["path"] = toml::array(path.begin(), path.end());
    }
}

void PZooConfig::setFullscreen(const QString &fullscreen) {
    if (m_fullscreen != fullscreen) {
        m_fullscreen = fullscreen;
        emit fullscreenChanged(m_fullscreen);

        m_dirty = true;
        m_configTable["user"]["fullscreen"] = fullscreen.toStdString();
    }
}

void PZooConfig::setScreenwidth(const QString &screenwidth) {
    if (m_screenwidth != screenwidth) {
        m_screenwidth = screenwidth;
        emit screenwidthChanged(m_screenwidth);

        m_dirty = true;
        m_configTable["user"]["screenwidth"] = screenwidth.toStdString();
    }
}

void PZooConfig::setScreenheight(const QString &screenheight) {
    if (m_screenheight != screenheight) {
        m_screenheight = screenheight;
        emit screenheightChanged(m_screenheight);

        m_dirty = true;
        m_configTable["user"]["screenheight"] = screenheight.toStdString();
    }
}

void PZooConfig::setUpdateRate(const QString &updateRate) {
    if (m_UpdateRate != updateRate) {
        m_UpdateRate = updateRate;
        emit updateRateChanged(m_UpdateRate);

        m_dirty = true;
        m_configTable["user"]["UpdateRate"] = updateRate.toStdString();
    }
}

void PZooConfig::setDrawRate(const QString &drawRate) {
    if (m_DrawRate != drawRate) {
        m_DrawRate = drawRate;
        emit drawRateChanged(m_DrawRate);

        m_dirty = true;
        m_configTable["user"]["DrawRate"] = drawRate.toStdString();
    }
}

void PZooConfig::setLastfile(const QString &lastfile) {
    if (m_lastfile != lastfile) {
        m_lastfile = lastfile;
        emit lastfileChanged(m_lastfile);

        m_dirty = true;
        m_configTable["user"]["lastfile"] = lastfile.toStdString();
    }
}

void PZooConfig::setUnlockCount(const QString &unlockCount) {
    if (m_unlockCount != unlockCount) {
        m_unlockCount = unlockCount;
        emit unlockCountChanged(m_unlockCount);

        m_dirty = true;
        m_configTable["user"]["unlockCount"] = unlockCount.toStdString();
    }
}

void PZooConfig::setUnlockEntity(const QStringList &unlockEntity) {
    if (m_unlockEntity != unlockEntity) {
        m_unlockEntity = unlockEntity;
        emit unlockEntityChanged(m_unlockEntity);

        m_dirty = true;
        m_configTable["user"]["unlockEntity"] = toml::array(unlockEntity.begin(), unlockEntity.end());
    }
}

void PZooConfig::setDrag(const QString &drag) {
    if (m_drag != drag) {
        m_drag = drag;
        emit dragChanged(m_drag);

        m_dirty = true;
        m_configTable["advanced"]["drag"] = drag.toStdString();
    }
}

void PZooConfig::setClick(const QString &click) {
    if (m_click != click) {
        m_click = click;
        emit clickChanged(m_click);

        m_dirty = true;
        m_configTable["advanced"]["click"] = click.toStdString();
    }
}

void PZooConfig::setNormal(const QString &normal) {
    if (m_normal != normal) {
        m_normal = normal;
        emit normalChanged(m_normal);

        m_dirty = true;
        m_configTable["advanced"]["normal"] = normal.toStdString();
    }
}

void PZooConfig::setLoadHalfAnims(const QString &loadHalfAnims) {
    if (m_loadHalfAnims != loadHalfAnims) {
        m_loadHalfAnims = loadHalfAnims;
        emit loadHalfAnimsChanged(m_loadHalfAnims);

        m_dirty = true;
        m_configTable["advanced"]["loadHalfAnims"] = loadHalfAnims.toStdString();
    }
}

void PZooConfig::setUse8BitSound(const QString &use8BitSound) {
    if (m_use8BitSound != use8BitSound) {
        m_use8BitSound = use8BitSound;
        emit use8BitSoundChanged(m_use8BitSound);

        m_dirty = true;
        m_configTable["advanced"]["use8BitSound"] = use8BitSound.toStdString();
    }
}

void PZooConfig::setLevel(const QString &level) {
    if (m_level != level) {
        m_level = level;
        emit levelChanged(m_level);

        m_dirty = true;
        m_configTable["advanced"]["level"] = level.toStdString();
    }
}

void PZooConfig::setDevModeEnabled(const QString &devModeEnabled) {
    if (m_devModeEnabled != devModeEnabled) {
        m_devModeEnabled = devModeEnabled;
        emit devModeEnabledChanged(m_devModeEnabled);

        m_dirty = true;
        m_configTable["advanced"]["devModeEnabled"] = devModeEnabled.toStdString();
    }
}

void PZooConfig::setMapX(const QString &mapX) {
    if (m_mapX != mapX) {
        m_mapX = mapX;
        emit mapXChanged(m_mapX);

        m_dirty = true;
        m_configTable["Map"]["x"] = mapX.toStdString();
    }
}

void PZooConfig::setMapY(const QString &mapY) {
    if (m_mapY != mapY) {
        m_mapY = mapY;
        emit mapYChanged(m_mapY);

        m_dirty = true;
        m_configTable["Map"]["y"] = mapY.toStdString();
    }
}

void PZooConfig::setUseAlternateCursors(const QString &useAlternateCursors) {
    if (m_useAlternateCursors != useAlternateCursors) {
        m_useAlternateCursors = useAlternateCursors;
        emit useAlternateCursorsChanged(m_useAlternateCursors);

        m_dirty = true;
        m_configTable["UI"]["useAlternateCursors"] = useAlternateCursors.toStdString();
    }
}

void PZooConfig::setTooltipDelay(const QString &tooltipDelay) {
    if (m_tooltipDelay != tooltipDelay) {
        m_tooltipDelay = tooltipDelay;
        emit tooltipDelayChanged(m_tooltipDelay);

        m_dirty = true;
        m_configTable["UI"]["tooltipDelay"] = tooltipDelay.toStdString();
    }
}

void PZooConfig::setTooltipDuration(const QString &tooltipDuration) {
    if (m_tooltipDuration != tooltipDuration) {
        m_tooltipDuration = tooltipDuration;
        emit tooltipDurationChanged(m_tooltipDuration);

        m_dirty = true;
        m_configTable["UI"]["tooltipDuration"] = tooltipDuration.toStdString();
    }
}

void PZooConfig::setMessageDisplay(const QString &messageDisplay) {
    if (m_MessageDisplay != messageDisplay) {
        m_MessageDisplay = messageDisplay;
        emit messageDisplayChanged(m_MessageDisplay);

        m_dirty = true;
        m_configTable["UI"]["messageDisplay"] = messageDisplay.toStdString();
    }
}

void PZooConfig::setMouseScrollThreshold(const QString &mouseScrollThreshold) {
    if (m_mouseScrollThreshold != mouseScrollThreshold) {
        m_mouseScrollThreshold = mouseScrollThreshold;
        emit mouseScrollThresholdChanged(m_mouseScrollThreshold);

        m_dirty = true;
        m_configTable["UI"]["mouseScrollThreshold"] = mouseScrollThreshold.toStdString();
    }
}

void PZooConfig::setMouseScrollDelay(const QString &mouseScrollDelay) {
    if (m_mouseScrollDelay != mouseScrollDelay) {
        m_mouseScrollDelay = mouseScrollDelay;
        emit mouseScrollDelayChanged(m_mouseScrollDelay);

        m_dirty = true;
        m_configTable["UI"]["mouseScrollDelay"] = mouseScrollDelay.toStdString();
    }
}

void PZooConfig::setMouseScrollX(const QString &mouseScrollX) {
    if (m_mouseScrollX != mouseScrollX) {
        m_mouseScrollX = mouseScrollX;
        emit mouseScrollXChanged(m_mouseScrollX);

        m_dirty = true;
        m_configTable["UI"]["mouseScrollX"] = mouseScrollX.toStdString();
    }
}

void PZooConfig::setMouseScrollY(const QString &mouseScrollY) {
    if (m_mouseScrollY != mouseScrollY) {
        m_mouseScrollY = mouseScrollY;
        emit mouseScrollYChanged(m_mouseScrollY);

        m_dirty = true;
        m_configTable["UI"]["mouseScrollY"] = mouseScrollY.toStdString();
    }
}

void PZooConfig::setKeyScrollX(const QString &keyScrollX) {
    if (m_keyScrollX != keyScrollX) {
        m_keyScrollX = keyScrollX;
        emit keyScrollXChanged(m_keyScrollX);

        m_dirty = true;
        m_configTable["UI"]["keyScrollX"] = keyScrollX.toStdString();
    }
}

void PZooConfig::setKeyScrollY(const QString &keyScrollY) {
    if (m_keyScrollY != keyScrollY) {
        m_keyScrollY = keyScrollY;
        emit keyScrollYChanged(m_keyScrollY);

        m_dirty = true;
        m_configTable["UI"]["keyScrollY"] = keyScrollY.toStdString();
    }
}

void PZooConfig::setMinimumMessageInterval(const QString &minimumMessageInterval) {
    if (m_minimumMessageInterval != minimumMessageInterval) {
        m_minimumMessageInterval = minimumMessageInterval;
        emit minimumMessageIntervalChanged(m_minimumMessageInterval);

        m_dirty = true;
        m_configTable["UI"]["minimumMessageInterval"] = minimumMessageInterval.toStdString();
    }
}

void PZooConfig::setDefaultEditCharLimit(const QString &defaultEditCharLimit) {
    if (m_defaultEditCharLimit != defaultEditCharLimit) {
        m_defaultEditCharLimit = defaultEditCharLimit;
        emit defaultEditCharLimitChanged(m_defaultEditCharLimit);

        m_dirty = true;
        m_configTable["UI"]["defaultEditCharLimit"] = defaultEditCharLimit.toStdString();
    }
}

void PZooConfig::setNoMenuMusic(const QString &noMenuMusic) {
    if (m_noMenuMusic != noMenuMusic) {
        m_noMenuMusic = noMenuMusic;
        emit noMenuMusicChanged(m_noMenuMusic);

        m_dirty = true;
        m_configTable["UI"]["noMenuMusic"] = noMenuMusic.toStdString();
    }
}

void PZooConfig::setMenuMusic(const QString &menuMusic) {
    if (m_menuMusic != menuMusic) {
        m_menuMusic = menuMusic;
        emit menuMusicChanged(m_menuMusic);

        m_dirty = true;
        m_configTable["UI"]["menuMusic"] = menuMusic.toStdString();
    }
}

void PZooConfig::setMenuMusicAttenuation(const QString &menuMusicAttenuation) {
    if (m_menuMusicAttenuation != menuMusicAttenuation) {
        m_menuMusicAttenuation = menuMusicAttenuation;
        emit menuMusicAttenuationChanged(m_menuMusicAttenuation);

        m_dirty = true;
        m_configTable["UI"]["menuMusicAttenuation"] = menuMusicAttenuation.toStdString();
    }
}

void PZooConfig::setUserAttenuation(const QString &userAttenuation) {
    if (m_userAttenuation != userAttenuation) {
        m_userAttenuation = userAttenuation;
        emit userAttenuationChanged(m_userAttenuation);

        m_dirty = true;
        m_configTable["UI"]["userAttenuation"] = userAttenuation.toStdString();
    }
}

void PZooConfig::setHelpType(const QString &helpType) {
    if (m_helpType != helpType) {
        m_helpType = helpType;
        emit helpTypeChanged(m_helpType);

        m_dirty = true;
        m_configTable["UI"]["helpType"] = helpType.toStdString();
    }
}

void PZooConfig::setPlayMovie(const QString &playMovie) {
    if (m_playMovie != playMovie) {
        m_playMovie = playMovie;
        emit playMovieChanged(m_playMovie);

        m_dirty = true;
        m_configTable["UI"]["playMovie"] = playMovie.toStdString();
    }
}

void PZooConfig::setMovievolume1(const QString &movievolume1) {
    if (m_movievolume1 != movievolume1) {
        m_movievolume1 = movievolume1;
        emit movievolume1Changed(m_movievolume1);

        m_dirty = true;
        m_configTable["UI"]["movievolume1"] = movievolume1.toStdString();
    }
}

void PZooConfig::setPlaySecondMovie(const QString &playSecondMovie) {
    if (m_playSecondMovie != playSecondMovie) {
        m_playSecondMovie = playSecondMovie;
        emit playSecondMovieChanged(m_playSecondMovie);

        m_dirty = true;
        m_configTable["UI"]["playSecondMovie"] = playSecondMovie.toStdString();
    }
}

void PZooConfig::setMovievolume2(const QString &movievolume2) {
    if (m_movievolume2 != movievolume2) {
        m_movievolume2 = movievolume2;
        emit movievolume2Changed(m_movievolume2);

        m_dirty = true;
        m_configTable["UI"]["movievolume2"] = movievolume2.toStdString();
    }
}

void PZooConfig::setMaxShortTooltipWidth(const QString &maxShortTooltipWidth) {
    if (m_maxShortTooltipWidth != maxShortTooltipWidth) {
        m_maxShortTooltipWidth = maxShortTooltipWidth;
        emit maxShortTooltipWidthChanged(m_maxShortTooltipWidth);

        m_dirty = true;
        m_configTable["UI"]["maxShortTooltipWidth"] = maxShortTooltipWidth.toStdString();
    }
}

void PZooConfig::setMaxLongTooltipWidth(const QString &maxLongTooltipWidth) {
    if (m_maxLongTooltipWidth != maxLongTooltipWidth) {
        m_maxLongTooltipWidth = maxLongTooltipWidth;
        emit maxLongTooltipWidthChanged(m_maxLongTooltipWidth);

        m_dirty = true;
        m_configTable["UI"]["maxLongTooltipWidth"] = maxLongTooltipWidth.toStdString();
    }
}

void PZooConfig::setProgressLeft(const QString &progressLeft) {
    if (m_progressLeft != progressLeft) {
        m_progressLeft = progressLeft;
        emit progressLeftChanged(m_progressLeft);

        m_dirty = true;
        m_configTable["UI"]["progressLeft"] = progressLeft.toStdString();
    }
}

void PZooConfig::setProgressTop(const QString &progressTop) {
    if (m_progressTop != progressTop) {
        m_progressTop = progressTop;
        emit progressTopChanged(m_progressTop);

        m_dirty = true;
        m_configTable["UI"]["progressTop"] = progressTop.toStdString();
    }
}

void PZooConfig::setProgressRight(const QString &progressRight) {
    if (m_progressRight != progressRight) {
        m_progressRight = progressRight;
        emit progressRightChanged(m_progressRight);

        m_dirty = true;
        m_configTable["UI"]["progressRight"] = progressRight.toStdString();
    }
}

void PZooConfig::setProgressBottom(const QString &progressBottom) {
    if (m_progressBottom != progressBottom) {
        m_progressBottom = progressBottom;
        emit progressBottomChanged(m_progressBottom);

        m_dirty = true;
        m_configTable["UI"]["progressBottom"] = progressBottom.toStdString();
    }
}

void PZooConfig::setProgressRed(const QString &progressRed) {
    if (m_progressRed != progressRed) {
        m_progressRed = progressRed;
        emit progressRedChanged(m_progressRed);

        m_dirty = true;
        m_configTable["UI"]["progressRed"] = progressRed.toStdString();
    }
}

void PZooConfig::setProgressGreen(const QString &progressGreen) {
    if (m_progressGreen != progressGreen) {
        m_progressGreen = progressGreen;
        emit progressGreenChanged(m_progressGreen);

        m_dirty = true;
        m_configTable["UI"]["progressGreen"] = progressGreen.toStdString();
    }
}

void PZooConfig::setProgressBlue(const QString &progressBlue) {
    if (m_progressBlue != progressBlue) {
        m_progressBlue = progressBlue;
        emit progressBlueChanged(m_progressBlue);

        m_dirty = true;
        m_configTable["UI"]["progressBlue"] = progressBlue.toStdString();
    }
}

void PZooConfig::setProgressShadowXOffset(const QString &progressShadowXOffset) {
    if (m_progressShadowXOffset != progressShadowXOffset) {
        m_progressShadowXOffset = progressShadowXOffset;
        emit progressShadowXOffsetChanged(m_progressShadowXOffset);

        m_dirty = true;
        m_configTable["UI"]["progressShadowXOffset"] = progressShadowXOffset.toStdString();
    }
}

void PZooConfig::setProgressShadowYOffset(const QString &progressShadowYOffset) {
    if (m_progressShadowYOffset != progressShadowYOffset) {
        m_progressShadowYOffset = progressShadowYOffset;
        emit progressShadowYOffsetChanged(m_progressShadowYOffset);

        m_dirty = true;
        m_configTable["UI"]["progressShadowYOffset"] = progressShadowYOffset.toStdString();
    }
}

void PZooConfig::setProgressShadowRed(const QString &progressShadowRed) {
    if (m_progressShadowRed != progressShadowRed) {
        m_progressShadowRed = progressShadowRed;
        emit progressShadowRedChanged(m_progressShadowRed);

        m_dirty = true;
        m_configTable["UI"]["progressShadowRed"] = progressShadowRed.toStdString();
    }
}

void PZooConfig::setProgressShadowGreen(const QString &progressShadowGreen) {
    if (m_progressShadowGreen != progressShadowGreen) {
        m_progressShadowGreen = progressShadowGreen;
        emit progressShadowGreenChanged(m_progressShadowGreen);

        m_dirty = true;
        m_configTable["UI"]["progressShadowGreen"] = progressShadowGreen.toStdString();
    }
}

void PZooConfig::setProgressShadowBlue(const QString &progressShadowBlue) {
    if (m_progressShadowBlue != progressShadowBlue) {
        m_progressShadowBlue = progressShadowBlue;
        emit progressShadowBlueChanged(m_progressShadowBlue);

        m_dirty = true;
        m_configTable["UI"]["progressShadowBlue"] = progressShadowBlue.toStdString();
    }
}

void PZooConfig::setCompletedExhibitAttenuation(const QString &completedExhibitAttenuation) {
    if (m_completedExhibitAttenuation != completedExhibitAttenuation) {
        m_completedExhibitAttenuation = completedExhibitAttenuation;
        emit completedExhibitAttenuationChanged(m_completedExhibitAttenuation);

        m_dirty = true;
        m_configTable["UI"]["completedExhibitAttenuation"] = completedExhibitAttenuation.toStdString();
    }
}

void PZooConfig::setMSStartingCash(const QString &MSStartingCash) {
    if (m_MSStartingCash != MSStartingCash) {
        m_MSStartingCash = MSStartingCash;
        emit MSStartingCashChanged(m_MSStartingCash);

        m_dirty = true;
        m_configTable["UI"]["MSStartingCash"] = MSStartingCash.toStdString();
    }
}

void PZooConfig::setMSCashIncrement(const QString &MSCashIncrement) {
    if (m_MSCashIncrement != MSCashIncrement) {
        m_MSCashIncrement = MSCashIncrement;
        emit MSCashIncrementChanged(m_MSCashIncrement);

        m_dirty = true;
        m_configTable["UI"]["MSCashIncrement"] = MSCashIncrement.toStdString();
    }
}

void PZooConfig::setMSMinCash(const QString &MSMinCash) {
    if (m_MSMinCash != MSMinCash) {
        m_MSMinCash = MSMinCash;
        emit MSMinCashChanged(m_MSMinCash);

        m_dirty = true;
        m_configTable["UI"]["MSMinCash"] = MSMinCash.toStdString();
    }
}

void PZooConfig::setMSMaxCash(const QString &MSMaxCash) {
    if (m_MSMaxCash != MSMaxCash) {
        m_MSMaxCash = MSMaxCash;
        emit MSMaxCashChanged(m_MSMaxCash);

        m_dirty = true;
        m_configTable["UI"]["MSMaxCash"] = MSMaxCash.toStdString();
    }
}

void PZooConfig::setStartedFirstTutorial(const QString &startedFirstTutorial) {
    if (m_startedFirstTutorial != startedFirstTutorial) {
        m_startedFirstTutorial = startedFirstTutorial;
        emit startedFirstTutorialChanged(m_startedFirstTutorial);

        m_dirty = true;
        m_configTable["UI"]["startedFirstTutorial"] = startedFirstTutorial.toStdString();
    }
}

void PZooConfig::setProgresscalls(const QString &progresscalls) {
    if (m_progresscalls != progresscalls) {
        m_progresscalls = progresscalls;
        emit progresscallsChanged(m_progresscalls);

        m_dirty = true;
        m_configTable["UI"]["progresscalls"] = progresscalls.toStdString();
    }
}

void PZooConfig::setStartedDinoTutorial(const QString &startedDinoTutorial) {
    if (m_startedDinoTutorial != startedDinoTutorial) {
        m_startedDinoTutorial = startedDinoTutorial;
        emit startedDinoTutorialChanged(m_startedDinoTutorial);

        m_dirty = true;
        m_configTable["UI"]["startedDinoTutorial"] = startedDinoTutorial.toStdString();
    }
}

void PZooConfig::setStartedAquaTutorial(const QString &startedAquaTutorial) {
    if (m_startedAquaTutorial != startedAquaTutorial) {
        m_startedAquaTutorial = startedAquaTutorial;
        emit startedAquaTutorialChanged(m_startedAquaTutorial);

        m_dirty = true;
        m_configTable["UI"]["startedAquaTutorial"] = startedAquaTutorial.toStdString();
    }
}

void PZooConfig::setLastWindowX(const QString &lastWindowX) {
    if (m_lastWindowX != lastWindowX) {
        m_lastWindowX = lastWindowX;
        emit lastWindowXChanged(m_lastWindowX);

        m_dirty = true;
        m_configTable["UI"]["lastWindowX"] = lastWindowX.toStdString();
    }
}

void PZooConfig::setLastWindowY(const QString &lastWindowY) {
    if (m_lastWindowY != lastWindowY) {
        m_lastWindowY = lastWindowY;
        emit lastWindowYChanged(m_lastWindowY);

        m_dirty = true;
        m_configTable["UI"]["lastWindowY"] = lastWindowY.toStdString();
    }
}

void PZooConfig::setTutorial(const QString &tutorial) {
    if (m_tutorial != tutorial) {
        m_tutorial = tutorial;
        emit tutorialChanged(m_tutorial);

        m_dirty = true;
        m_configTable["scenario"]["tutorial"] = tutorial.toStdString();
    }
}

void PZooConfig::setAa(const QString &aa) {
    if (m_aa != aa) {
        m_aa = aa;
        emit aaChanged(m_aa);

        m_dirty = true;
        m_configTable["scenario"]["aa"] = aa.toStdString();
    }
}

void PZooConfig::setAb(const QString &ab) {
    if (m_ab != ab) {
        m_ab = ab;
        emit abChanged(m_ab);

        m_dirty = true;
        m_configTable["scenario"]["ab"] = ab.toStdString();
    }
}

void PZooConfig::setAc(const QString &ac) {
    if (m_ac != ac) {
        m_ac = ac;
        emit acChanged(m_ac);

        m_dirty = true;
        m_configTable["scenario"]["ac"] = ac.toStdString();
    }
}

void PZooConfig::setAd(const QString &ad) {
    if (m_ad != ad) {
        m_ad = ad;
        emit adChanged(m_ad);

        m_dirty = true;
        m_configTable["scenario"]["ad"] = ad.toStdString();
    }
}

void PZooConfig::setAe(const QString &ae) {
    if (m_ae != ae) {
        m_ae = ae;
        emit aeChanged(m_ae);

        m_dirty = true;
        m_configTable["scenario"]["ae"] = ae.toStdString();
    }
}

void PZooConfig::setAf(const QString &af) {
    if (m_af != af) {
        m_af = af;
        emit afChanged(m_af);

        m_dirty = true;
        m_configTable["scenario"]["af"] = af.toStdString();
    }
}

void PZooConfig::setAg(const QString &ag) {
    if (m_ag != ag) {
        m_ag = ag;
        emit agChanged(m_ag);

        m_dirty = true;
        m_configTable["scenario"]["ag"] = ag.toStdString();
    }
}

void PZooConfig::setBa(const QString &ba) {
    if (m_ba != ba) {
        m_ba = ba;
        emit baChanged(m_ba);

        m_dirty = true;
        m_configTable["scenario"]["ba"] = ba.toStdString();
    }
}

void PZooConfig::setBb(const QString &bb) {
    if (m_bb != bb) {
        m_bb = bb;
        emit bbChanged(m_bb);

        m_dirty = true;
        m_configTable["scenario"]["bb"] = bb.toStdString();
    }
}

void PZooConfig::setBc(const QString &bc) {
    if (m_bc != bc) {
        m_bc = bc;
        emit bcChanged(m_bc);

        m_dirty = true;
        m_configTable["scenario"]["bc"] = bc.toStdString();
    }
}

void PZooConfig::setBd(const QString &bd) {
    if (m_bd != bd) {
        m_bd = bd;
        emit bdChanged(m_bd);

        m_dirty = true;
        m_configTable["scenario"]["bd"] = bd.toStdString();
    }
}

void PZooConfig::setBe(const QString &be) {
    if (m_be != be) {
        m_be = be;
        emit beChanged(m_be);

        m_dirty = true;
        m_configTable["scenario"]["be"] = be.toStdString();
    }
}

void PZooConfig::setBf(const QString &bf) {
    if (m_bf != bf) {
        m_bf = bf;
        emit bfChanged(m_bf);

        m_dirty = true;
        m_configTable["scenario"]["bf"] = bf.toStdString();
    }
}

void PZooConfig::setBg(const QString &bg) {
    if (m_bg != bg) {
        m_bg = bg;
        emit bgChanged(m_bg);

        m_dirty = true;
        m_configTable["scenario"]["bg"] = bg.toStdString();
    }
}

void PZooConfig::setCa(const QString &ca) {
    if (m_ca != ca) {
        m_ca = ca;
        emit caChanged(m_ca);

        m_dirty = true;
        m_configTable["scenario"]["ca"] = ca.toStdString();
    }
}

void PZooConfig::setCb(const QString &cb) {
    if (m_cb != cb) {
        m_cb = cb;
        emit cbChanged(m_cb);

        m_dirty = true;
        m_configTable["scenario"]["cb"] = cb.toStdString();
    }
}

void PZooConfig::setCc(const QString &cc) {
    if (m_cc != cc) {
        m_cc = cc;
        emit ccChanged(m_cc);

        m_dirty = true;
        m_configTable["scenario"]["cc"] = cc.toStdString();
    }
}

void PZooConfig::setCd(const QString &cd) {
    if (m_cd != cd) {
        m_cd = cd;
        emit cdChanged(m_cd);

        m_dirty = true;
        m_configTable["scenario"]["cd"] = cd.toStdString();
    }
}

void PZooConfig::setCe(const QString &ce) {
    if (m_ce != ce) {
        m_ce = ce;
        emit ceChanged(m_ce);

        m_dirty = true;
        m_configTable["scenario"]["ce"] = ce.toStdString();
    }
}

void PZooConfig::setCf(const QString &cf) {
    if (m_cf != cf) {
        m_cf = cf;
        emit cfChanged(m_cf);

        m_dirty = true;
        m_configTable["scenario"]["cf"] = cf.toStdString();
    }
}

void PZooConfig::setCg(const QString &cg) {
    if (m_cg != cg) {
        m_cg = cg;
        emit cgChanged(m_cg);

        m_dirty = true;
        m_configTable["scenario"]["cg"] = cg.toStdString();
    }
}

void PZooConfig::setDa(const QString &da) {
    if (m_da != da) {
        m_da = da;
        emit daChanged(m_da);

        m_dirty = true;
        m_configTable["scenario"]["da"] = da.toStdString();
    }
}

void PZooConfig::setDb(const QString &db) {
    if (m_db != db) {
        m_db = db;
        emit dbChanged(m_db);

        m_dirty = true;
        m_configTable["scenario"]["db"] = db.toStdString();
    }
}

void PZooConfig::setDc(const QString &dc) {
    if (m_dc != dc) {
        m_dc = dc;
        emit dcChanged(m_dc);

        m_dirty = true;
        m_configTable["scenario"]["dc"] = dc.toStdString();
    }
}

void PZooConfig::setDd(const QString &dd) {
    if (m_dd != dd) {
        m_dd = dd;
        emit ddChanged(m_dd);

        m_dirty = true;
        m_configTable["scenario"]["dd"] = dd.toStdString();
    }
}

void PZooConfig::setDe(const QString &de) {
    if (m_de != de) {
        m_de = de;
        emit deChanged(m_de);

        m_dirty = true;
        m_configTable["scenario"]["de"] = de.toStdString();
    }
}

void PZooConfig::setDf(const QString &df) {
    if (m_df != df) {
        m_df = df;
        emit dfChanged(m_df);

        m_dirty = true;
        m_configTable["scenario"]["df"] = df.toStdString();
    }
}

void PZooConfig::setDg(const QString &dg) {
    if (m_dg != dg) {
        m_dg = dg;
        emit dgChanged(m_dg);

        m_dirty = true;
        m_configTable["scenario"]["dg"] = dg.toStdString();
    }
}

void PZooConfig::setDh(const QString &dh) {
    if (m_dh != dh) {
        m_dh = dh;
        emit dhChanged(m_dh);

        m_dirty = true;
        m_configTable["scenario"]["dh"] = dh.toStdString();
    }
}

void PZooConfig::setEa(const QString &ea) {
    if (m_ea != ea) {
        m_ea = ea;
        emit eaChanged(m_ea);

        m_dirty = true;
        m_configTable["scenario"]["ea"] = ea.toStdString();
    }
}   

void PZooConfig::setFa(const QString &fa) {
    if (m_fa != fa) {
        m_fa = fa;
        emit faChanged(m_fa);

        m_dirty = true;
        m_configTable["scenario"]["fa"] = fa.toStdString();
    }
}

void PZooConfig::setFb(const QString &fb) {
    if (m_fb != fb) {
        m_fb = fb;
        emit fbChanged(m_fb);

        m_dirty = true;
        m_configTable["scenario"]["fb"] = fb.toStdString();
    }
}

void PZooConfig::setGa(const QString &ga) {
    if (m_ga != ga) {
        m_ga = ga;
        emit gaChanged(m_ga);

        m_dirty = true;
        m_configTable["scenario"]["ga"] = ga.toStdString();
    }
}

void PZooConfig::setGb(const QString &gb) {
    if (m_gb != gb) {
        m_gb = gb;
        emit gbChanged(m_gb);

        m_dirty = true;
        m_configTable["scenario"]["gb"] = gb.toStdString();
    }
}

void PZooConfig::setGc(const QString &gc) {
    if (m_gc != gc) {
        m_gc = gc;
        emit gcChanged(m_gc);

        m_dirty = true;
        m_configTable["scenario"]["gc"] = gc.toStdString();
    }
}

void PZooConfig::setGd(const QString &gd) {
    if (m_gd != gd) {
        m_gd = gd;
        emit gdChanged(m_gd);

        m_dirty = true;
        m_configTable["scenario"]["gd"] = gd.toStdString();
    }
}


