#ifndef PZOOCONFIG_H
#define PZOOCONFIG_H

#include "toml.hpp"
#include "PZtdMgr.h"
#include <QSettings>
#include <QDir>
#include "../src/PConfigMgr.h"

class PZooConfig : public QObject {
    Q_OBJECT

public:
    explicit PZooConfig(QObject *parent = nullptr) : QObject(parent) {}
    PZooConfig(const PZooConfig&) = delete;
    PZooConfig& operator=(const PZooConfig&) = delete;
    PZooConfig(PZooConfig&&) = default;

    QString getZooConfigPath() const { return m_zooConfigPath; }
    void setZooConfigPath(const QString &path);

    // ---------------------------------------------------- [debug]

    QString getLogCutoff() const { return m_logCutoff; }
    void setLogCutoff(const QString &logCutoff) { m_logCutoff = logCutoff; }

    QString getSendLogfile() const { return m_sendLogfile; }
    void setSendLogfile(const QString &sendLogfile) { m_sendLogfile = sendLogfile; }

    QString getSendDebugger() const { return m_sendDebugger; }
    void setSendDebugger(const QString &sendDebugger) { m_sendDebugger = sendDebugger; }

    QString getDeltaLog1() const { return m_deltaLog1; }
    void setDeltaLog1(const QString &deltaLog1) { m_deltaLog1 = deltaLog1; }

    QString getDeltaLog0() const { return m_deltaLog0; }
    void setDeltaLog0(const QString &deltaLog0) { m_deltaLog0 = deltaLog0; }

    QString getDrawfps() const { return m_drawfps; }
    void setDrawfps(const QString &drawfps) { m_drawfps = drawfps; }

    QString getDrawfpsx() const { return m_drawfpsx; }
    void setDrawfpsx(const QString &drawfpsx) { m_drawfpsx = drawfpsx; }

    QString getDrawfpsy() const { return m_drawfpsy; }
    void setDrawfpsy(const QString &drawfpsy) { m_drawfpsy = drawfpsy; }

    // ---------------------------------------------------- [mgr]

    QString getAimgr() const { return m_aimgr; }
    void setAimgr(const QString &aimgr) { m_aimgr = aimgr; }

    QString getWorldmgr() const { return m_worldmgr; }
    void setWorldmgr(const QString &worldmgr) { m_worldmgr = worldmgr; }

    QString getGamemgr() const { return m_gamemgr; }
    void setGamemgr(const QString &gamemgr) { m_gamemgr = gamemgr; }

    QString getScenariomgr() const { return m_scenariomgr; }
    void setScenariomgr(const QString &scenariomgr) { m_scenariomgr = scenariomgr; }

    QString getScriptmgr() const { return m_scriptmgr; }
    void setScriptmgr(const QString &scriptmgr) { m_scriptmgr = scriptmgr; }

    QString getSoundmgr() const { return m_soundmgr; }
    void setSoundmgr(const QString &soundmgr) { m_soundmgr = soundmgr; }

    QString getTerrainmgr() const { return m_terrainmgr; }
    void setTerrainmgr(const QString &terrainmgr) { m_terrainmgr = terrainmgr; }

    // ---------------------------------------------------- [language]

    QString getLang() const { return m_lang; }
    void setLang(const QString &lang) { m_lang = lang; }

    QString getSublang() const { return m_sublang; }
    void setSublang(const QString &sublang) { m_sublang = sublang; }

    // ---------------------------------------------------- [lib]

    QString getRes() const { return m_res; }
    void setRes(const QString &res) { m_res = res; }

    QString getLangLib() const { return m_langLib; }
    void setLangLib(const QString &langLib) { m_langLib = langLib; }

    // ---------------------------------------------------- [resource]

    QString getPath() const { return m_path; }
    void setPath(const QString &path) { m_path = path; }

    // ---------------------------------------------------- [user]

    QString getFullscreen() const { return m_fullscreen; }
    void setFullscreen(const QString &fullscreen) { m_fullscreen = fullscreen; }

    QString getScreenwidth() const { return m_screenwidth; }
    void setScreenwidth(const QString &screenwidth) { m_screenwidth = screenwidth; }

    QString getScreenheight() const { return m_screenheight; }
    void setScreenheight(const QString &screenheight) { m_screenheight = screenheight; }

    QString getUpdateRate() const { return m_UpdateRate; }
    void setUpdateRate(const QString &updateRate) { m_UpdateRate = updateRate; }

    QString getDrawRate() const { return m_DrawRate; }
    void setDrawRate(const QString &drawRate) { m_DrawRate = drawRate; }

    QString getLastfile() const { return m_lastfile; }
    void setLastfile(const QString &lastfile) { m_lastfile = lastfile; }

    QString getUnlockCount() const { return m_unlockCount; }
    void setUnlockCount(const QString &unlockCount) { m_unlockCount = unlockCount; }

    QStringList getUnlockEntity() const { return m_unlockEntity; }
    void setUnlockEntity(const QStringList &unlockEntity) { m_unlockEntity = unlockEntity; }

    void addUnlockEntity(const QString &unlockEntity) { m_unlockEntity.append(unlockEntity); }
    void removeUnlockEntity(const QString &unlockEntity) { m_unlockEntity.removeAll(unlockEntity); }

    // ---------------------------------------------------- [advanced]

    QString getDrag() const { return m_drag; }
    void setDrag(const QString &drag) { m_drag = drag; }

    QString getClick() const { return m_click; }
    void setClick(const QString &click) { m_click = click; }

    QString getNormal() const { return m_normal; }
    void setNormal(const QString &normal) { m_normal = normal; }

    QString getLoadHalfAnims() const { return m_loadHalfAnims; }
    void setLoadHalfAnims(const QString &loadHalfAnims) { m_loadHalfAnims = loadHalfAnims; }

    QString getUse8BitSound() const { return m_use8BitSound; }
    void setUse8BitSound(const QString &use8BitSound) { m_use8BitSound = use8BitSound; }

    QString getLevel() const { return m_level; }
    void setLevel(const QString &level) { m_level = level; }

    QString getDevModeEnabled() const { return m_devModeEnabled; }
    void setDevModeEnabled(const QString &devModeEnabled) { m_devModeEnabled = devModeEnabled; }

    // ---------------------------------------------------- [Map]

    QString getMapX() const { return m_mapX; }
    void setMapX(const QString &mapX) { m_mapX = mapX; }

    QString getMapY() const { return m_mapY; }
    void setMapY(const QString &mapY) { m_mapY = mapY; }

    // ---------------------------------------------------- [UI]

    QString getUseAlternateCursors() const { return m_useAlternateCursors; }
    void setUseAlternateCursors(const QString &useAlternateCursors) { m_useAlternateCursors = useAlternateCursors; }

    QString getTooltipDelay() const { return m_tooltipDelay; }
    void setTooltipDelay(const QString &tooltipDelay) { m_tooltipDelay = tooltipDelay; }

    QString getTooltipDuration() const { return m_tooltipDuration; }
    void setTooltipDuration(const QString &tooltipDuration) { m_tooltipDuration = tooltipDuration; }

    QString getMessageDisplay() const { return m_MessageDisplay; }
    void setMessageDisplay(const QString &messageDisplay) { m_MessageDisplay = messageDisplay; }

    QString getMouseScrollThreshold() const { return m_mouseScrollThreshold; }
    void setMouseScrollThreshold(const QString &mouseScrollThreshold) { m_mouseScrollThreshold = mouseScrollThreshold; }

    QString getMouseScrollDelay() const { return m_mouseScrollDelay; }
    void setMouseScrollDelay(const QString &mouseScrollDelay) { m_mouseScrollDelay = mouseScrollDelay; }

    QString getMouseScrollX() const { return m_mouseScrollX; }
    void setMouseScrollX(const QString &mouseScrollX) { m_mouseScrollX = mouseScrollX; }

    QString getMouseScrollY() const { return m_mouseScrollY; }
    void setMouseScrollY(const QString &mouseScrollY) { m_mouseScrollY = mouseScrollY; }

    QString getKeyScrollX() const { return m_keyScrollX; }
    void setKeyScrollX(const QString &keyScrollX) { m_keyScrollX = keyScrollX; }

    QString getKeyScrollY() const { return m_keyScrollY; }
    void setKeyScrollY(const QString &keyScrollY) { m_keyScrollY = keyScrollY; }

    QString getMinimumMessageInterval() const { return m_minimumMessageInterval; }
    void setMinimumMessageInterval(const QString &minimumMessageInterval) { m_minimumMessageInterval = minimumMessageInterval; }

    QString getDefaultEditCharLimit() const { return m_defaultEditCharLimit; }
    void setDefaultEditCharLimit(const QString &defaultEditCharLimit) { m_defaultEditCharLimit = defaultEditCharLimit; }

    QString getNoMenuMusic() const { return m_noMenuMusic; }
    void setNoMenuMusic(const QString &noMenuMusic) { m_noMenuMusic = noMenuMusic; }

    QString getMenuMusic() const { return m_menuMusic; }
    void setMenuMusic(const QString &menuMusic) { m_menuMusic = menuMusic; }

    QString getMenuMusicAttenuation() const { return m_menuMusicAttenuation; }
    void setMenuMusicAttenuation(const QString &menuMusicAttenuation) { m_menuMusicAttenuation = menuMusicAttenuation; }

    QString getUserAttenuation() const { return m_userAttenuation; }
    void setUserAttenuation(const QString &userAttenuation) { m_userAttenuation = userAttenuation; }

    QString getHelpType() const { return m_helpType; }
    void setHelpType(const QString &helpType) { m_helpType = helpType; }

    QString getPlayMovie() const { return m_playMovie; }
    void setPlayMovie(const QString &playMovie) { m_playMovie = playMovie; }

    QString getMovievolume1() const { return m_movievolume1; }
    void setMovievolume1(const QString &movievolume1) { m_movievolume1 = movievolume1; }

    QString getPlaySecondMovie() const { return m_playSecondMovie; }
    void setPlaySecondMovie(const QString &playSecondMovie) { m_playSecondMovie = playSecondMovie; }

    QString getMovievolume2() const { return m_movievolume2; }
    void setMovievolume2(const QString &movievolume2) { m_movievolume2 = movievolume2; }

    QString getMaxShortTooltipWidth() const { return m_maxShortTooltipWidth; }
    void setMaxShortTooltipWidth(const QString &maxShortTooltipWidth) { m_maxShortTooltipWidth = maxShortTooltipWidth; }

    QString getMaxLongTooltipWidth() const { return m_maxLongTooltipWidth; }
    void setMaxLongTooltipWidth(const QString &maxLongTooltipWidth) { m_maxLongTooltipWidth = maxLongTooltipWidth; }

    QString getProgressLeft() const { return m_progressLeft; }
    void setProgressLeft(const QString &progressLeft) { m_progressLeft = progressLeft; }

    QString getProgressTop() const { return m_progressTop; }
    void setProgressTop(const QString &progressTop) { m_progressTop = progressTop; }

    QString getProgressRight() const { return m_progressRight; }
    void setProgressRight(const QString &progressRight) { m_progressRight = progressRight; }

    QString getProgressBottom() const { return m_progressBottom; }
    void setProgressBottom(const QString &progressBottom) { m_progressBottom = progressBottom; }

    QString getProgressRed() const { return m_progressRed; }
    void setProgressRed(const QString &progressRed) { m_progressRed = progressRed; }

    QString getProgressGreen() const { return m_progressGreen; }
    void setProgressGreen(const QString &progressGreen) { m_progressGreen = progressGreen; }

    QString getProgressBlue() const { return m_progressBlue; }
    void setProgressBlue(const QString &progressBlue) { m_progressBlue = progressBlue; }

    QString getProgressShadowXOffset() const { return m_progressShadowXOffset; }
    void setProgressShadowXOffset(const QString &progressShadowXOffset) { m_progressShadowXOffset = progressShadowXOffset; }

    QString getProgressShadowYOffset() const { return m_progressShadowYOffset; }
    void setProgressShadowYOffset(const QString &progressShadowYOffset) { m_progressShadowYOffset = progressShadowYOffset; }

    QString getProgressShadowRed() const { return m_progressShadowRed; }
    void setProgressShadowRed(const QString &progressShadowRed) { m_progressShadowRed = progressShadowRed; }

    QString getProgressShadowGreen() const { return m_progressShadowGreen; }
    void setProgressShadowGreen(const QString &progressShadowGreen) { m_progressShadowGreen = progressShadowGreen; }

    QString getProgressShadowBlue() const { return m_progressShadowBlue; }
    void setProgressShadowBlue(const QString &progressShadowBlue) { m_progressShadowBlue = progressShadowBlue; }

    QString getCompletedExhibitAttenuation() const { return m_completedExhibitAttenuation; }
    void setCompletedExhibitAttenuation(const QString &completedExhibitAttenuation) { m_completedExhibitAttenuation = completedExhibitAttenuation; }

    QString getMSStartingCash() const { return m_MSStartingCash; }
    void setMSStartingCash(const QString &MSStartingCash) { m_MSStartingCash = MSStartingCash; }

    QString getMSCashIncrement() const { return m_MSCashIncrement; }
    void setMSCashIncrement(const QString &MSCashIncrement) { m_MSCashIncrement = MSCashIncrement; }

    QString getMSMinCash() const { return m_MSMinCash; }
    void setMSMinCash(const QString &MSMinCash) { m_MSMinCash = MSMinCash; }

    QString getMSMaxCash() const { return m_MSMaxCash; }
    void setMSMaxCash(const QString &MSMaxCash) { m_MSMaxCash = MSMaxCash; }

    QString getStartedFirstTutorial() const { return m_startedFirstTutorial; }
    void setStartedFirstTutorial(const QString &startedFirstTutorial) { m_startedFirstTutorial = startedFirstTutorial; }

    QString getProgresscalls() const { return m_progresscalls; }
    void setProgresscalls(const QString &progresscalls) { m_progresscalls = progresscalls; }

    QString getStartedDinoTutorial() const { return m_startedDinoTutorial; }
    void setStartedDinoTutorial(const QString &startedDinoTutorial) { m_startedDinoTutorial = startedDinoTutorial; }

    QString getStartedAquaTutorial() const { return m_startedAquaTutorial; }
    void setStartedAquaTutorial(const QString &startedAquaTutorial) { m_startedAquaTutorial = startedAquaTutorial; }

    QString getLastWindowX() const { return m_lastWindowX; }
    void setLastWindowX(const QString &lastWindowX) { m_lastWindowX = lastWindowX; }

    QString getLastWindowY() const { return m_lastWindowY; }
    void setLastWindowY(const QString &lastWindowY) { m_lastWindowY = lastWindowY; }

    // ---------------------------------------------------- [scenario]
    QString getTutorial() const { return m_tutorial; }
    void setTutorial(const QString &tutorial) { m_tutorial = tutorial; }

    QString getAa() const { return m_aa; }
    void setAa(const QString &aa) { m_aa = aa; }

    QString getAb() const { return m_ab; }
    void setAb(const QString &ab) { m_ab = ab; }

    QString getAc() const { return m_ac; }
    void setAc(const QString &ac) { m_ac = ac; }

    QString getAd() const { return m_ad; }
    void setAd(const QString &ad) { m_ad = ad; }

    QString getAe() const { return m_ae; }
    void setAe(const QString &ae) { m_ae = ae; }

    QString getAf() const { return m_af; }
    void setAf(const QString &af) { m_af = af; }

    QString getAg() const { return m_ag; }
    void setAg(const QString &ag) { m_ag = ag; }

    QString getBa() const { return m_ba; }
    void setBa(const QString &ba) { m_ba = ba; }

    QString getBb() const { return m_bb; }
    void setBb(const QString &bb) { m_bb = bb; }

    QString getBc() const { return m_bc; }
    void setBc(const QString &bc) { m_bc = bc; }

    QString getBd() const { return m_bd; }
    void setBd(const QString &bd) { m_bd = bd; }

    QString getBe() const { return m_be; }
    void setBe(const QString &be) { m_be = be; }

    QString getBf() const { return m_bf; }
    void setBf(const QString &bf) { m_bf = bf; }

    QString getBg() const { return m_bg; }
    void setBg(const QString &bg) { m_bg = bg; }

    QString getBh() const { return m_bh; }
    void setBh(const QString &bh) { m_bh = bh; }

    QString getBi() const { return m_bi; }
    void setBi(const QString &bi) { m_bi = bi; }

    QString getCa() const { return m_ca; }
    void setCa(const QString &ca) { m_ca = ca; }

    QString getCb() const { return m_cb; }
    void setCb(const QString &cb) { m_cb = cb; }

    QString getCc() const { return m_cc; }
    void setCc(const QString &cc) { m_cc = cc; }

    QString getCd() const { return m_cd; }
    void setCd(const QString &cd) { m_cd = cd; }

    QString getCe() const { return m_ce; }
    void setCe(const QString &ce) { m_ce = ce; }

    QString getCf() const { return m_cf; }
    void setCf(const QString &cf) { m_cf = cf; }

    QString getCg() const { return m_cg; }
    void setCg(const QString &cg) { m_cg = cg; }

    QString getCh() const { return m_ch; }
    void setCh(const QString &ch) { m_ch = ch; }
    
    QString getCi() const { return m_ci; }
    void setCi(const QString &ci) { m_ci = ci; }

    QString getCj() const { return m_cj; }
    void setCj(const QString &cj) { m_cj = cj; }

    QString getCk() const { return m_ck; }
    void setCk(const QString &ck) { m_ck = ck; }

    QString getCl() const { return m_cl; }
    void setCl(const QString &cl) { m_cl = cl; }

    QString getCm() const { return m_cm; }
    void setCm(const QString &cm) { m_cm = cm; }

    QString getCn() const { return m_cn; }
    void setCn(const QString &cn) { m_cn = cn; }

    QString getDa() const { return m_da; }
    void setDa(const QString &da) { m_da = da; }

    QString getDb() const { return m_db; }
    void setDb(const QString &db) { m_db = db; }

    QString getDc() const { return m_dc; }
    void setDc(const QString &dc) { m_dc = dc; }

    QString getDd() const { return m_dd; }
    void setDd(const QString &dd) { m_dd = dd; }

    QString getDe() const { return m_de; }
    void setDe(const QString &de) { m_de = de; }

    QString getDf() const { return m_df; }
    void setDf(const QString &df) { m_df = df; }

    QString getDg() const { return m_dg; }
    void setDg(const QString &dg) { m_dg = dg; }

    QString getDh() const { return m_dh; }
    void setDh(const QString &dh) { m_dh = dh; }

    QString getEa() const { return m_ea; }
    void setEa(const QString &ea) { m_ea = ea; }

    QString getFa() const { return m_fa; }
    void setFa(const QString &fa) { m_fa = fa; }

    QString getFb() const { return m_fb; }
    void setFb(const QString &fb) { m_fb = fb; }

    QString getGa() const { return m_ga; }
    void setGa(const QString &ga) { m_ga = ga; }

    QString getGb() const { return m_gb; }
    void setGb(const QString &gb) { m_gb = gb; }

    QString getGc() const { return m_gc; }
    void setGc(const QString &gc) { m_gc = gc; }

    QString getGd() const { return m_gd; }
    void setGd(const QString &gd) { m_gd = gd; }

    // ---------------------------------------------------- [ai]
    QString getShowAIInfo() const { return m_showAIInfo; }
    void setShowAIInfo(const QString &showAIInfo) { m_showAIInfo = showAIInfo; }

    QString getShowName() const { return m_showName; }
    void setShowName(const QString &showName) { m_showName = showName; }

    QString getShowPosition() const { return m_showPosition; }
    void setShowPosition(const QString &showPosition) { m_showPosition = showPosition; }

    QString getShowAnimalAIInfo() const { return m_showAnimalAIInfo; }
    void setShowAnimalAIInfo(const QString &showAnimalAIInfo) { m_showAnimalAIInfo = showAnimalAIInfo; }

    QString getShowGuestAIInfo() const { return m_showGuestAIInfo; }
    void setShowGuestAIInfo(const QString &showGuestAIInfo) { m_showGuestAIInfo = showGuestAIInfo; }

    QString getShowStaffAIInfo() const { return m_showStaffAIInfo; }
    void setShowStaffAIInfo(const QString &showStaffAIInfo) { m_showStaffAIInfo = showStaffAIInfo; }

    QString getShowStatusVars() const { return m_showStatusVars; }
    void setShowStatusVars(const QString &showStatusVars) { m_showStatusVars = showStatusVars; }

    QString getShowFunctionCall() const { return m_showFunctionCall; }
    void setShowFunctionCall(const QString &showFunctionCall) { m_showFunctionCall = showFunctionCall; }

    QString getShowEvents() const { return m_showEvents; }
    void setShowEvents(const QString &showEvents) { m_showEvents = showEvents; }

    QString getShowBuildingAIInfo() const { return m_showBuildingAIInfo; }
    void setShowBuildingAIInfo(const QString &showBuildingAIInfo) { m_showBuildingAIInfo = showBuildingAIInfo; }

    Q_INVOKABLE void PZooConfig resetZooConfig();
private:
    QString m_zooConfigPath;
    // [debug]
    QString m_logCutoff;
    QString m_sendLogfile;
    QString m_sendDebugger;
    QString m_deltaLog1;
    QString m_deltaLog0;
    QString m_drawfps;
    QString m_drawfpsx;
    QString m_drawfpsy;
    // [mgr]
    QString m_aimgr;
    QString m_worldmgr;
    QString m_gamemgr;
    QString m_scenariomgr;
    QString m_scriptmgr;
    QString m_soundmgr;
    QString m_terrainmgr;
    // [language]
    QString m_lang;
    QString m_sublang;
    // [lib]
    QString m_res;
    QString m_langLib; // this is actually just "lang", but its different than the one in [language]

    // [resource]
    QString m_path;

    // [user]
    QString m_fullscreen;
    QString m_screenwidth;
    QString m_screenheight;
    QString m_UpdateRate;
    QString m_DrawRate;
    QString m_lastfile;
    QString m_unlockCount;
    QStringList m_unlockEntity; // unlockEntity0, unlockEntity1, up to an upper limit of 999
    // TODO: should check if there are duplicates in this list and remove them

    // [advanced]
    QString m_drag;
    QString m_click;
    QString m_normal;
    QString m_loadHalfAnims;
    QString m_use8BitSound;
    QString m_level;
    QString m_devModeEnabled;

    // [Map]
    QString m_mapX;
    QString m_mapY;

    // [UI]
    QString m_useAlternateCursors;
    QString m_tooltipDelay;
    QString m_tooltipDuration;
    QString m_MessageDisplay;
    QString m_mouseScrollThreshold;
    QString m_mouseScrollDelay; 
    QString m_mouseScrollX;
    QString m_mouseScrollY;
    QString m_keyScrollX;
    QString m_keyScrollY;
    QString m_minimumMessageInterval;
    QString m_defaultEditCharLimit;
    QString m_noMenuMusic;
    QString m_menuMusic;
    QString m_menuMusicAttenuation;
    QString m_userAttenuation;
    QString m_helpType;
    QString m_playMovie;
    QString m_movievolume1;
    QString m_playSecondMovie;
    QString m_movievolume2;
    QString m_maxShortTooltipWidth;
    QString m_maxLongTooltipWidth;
    QString m_progressLeft;
    QString m_progressTop;
    QString m_progressRight;
    QString m_progressBottom;
    QString m_progressRed;
    QString m_progressGreen;
    QString m_progressBlue;
    QString m_progressShadowXOffset;
    QString m_progressShadowYOffset;
    QString m_progressShadowRed;
    QString m_progressShadowGreen;
    QString m_progressShadowBlue;
    QString m_completedExhibitAttenuation;
    QString m_MSStartingCash;
    QString m_MSCashIncrement;
    QString m_MSMinCash;
    QString m_MSMaxCash;
    QString m_startedFirstTutorial;
    QString m_progresscalls;
    QString m_startedDinoTutorial;
    QString m_startedAquaTutorial;
    QString m_lastWindowX;
    QString m_lastWindowY;

    // [scenario]
    QString m_tutorial;
    QString m_aa;
    QString m_ab;
    QString m_ac;
    QString m_ad;
    QString m_ae;
    QString m_af;
    QString m_ag;
    QString m_ba;
    QString m_bb;
    QString m_bc;
    QString m_bd;
    QString m_be;
    QString m_bf;
    QString m_bg;
    QString m_bh;
    QString m_bi;
    QString m_ca;
    QString m_cb;
    QString m_cc;
    QString m_cd;
    QString m_ce;
    QString m_cf;
    QString m_cg;
    QString m_ch;
    QString m_ci;
    QString m_cj;
    QString m_ck;
    QString m_cl;
    QString m_cm;
    QString m_cn;
    QString m_da;
    QString m_db;
    QString m_dc;
    QString m_dd;
    QString m_de;
    QString m_df;
    QString m_dg;
    QString m_dh;
    QString m_ea;
    QString m_fa;
    QString m_fb;
    QString m_ga;
    QString m_gb;
    QString m_gc;
    QString m_gd;

    // [ai]
    QString m_showAIInfo;
    QString m_showName;
    QString m_showPosition;
    QString m_showAnimalAIInfo;
    QString m_showGuestAIInfo;
    QString m_showStaffAIInfo;
    QString m_showStatusVars;
    QString m_showFunctionCall;
    QString m_showEvents;
    QString m_showBuildingAIInfo;
    QString m_showSelected;
    QString m_showFrame;
    QString m_showGoal;

};

#endif // PZOOCONFIG_H