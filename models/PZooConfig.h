#ifndef PZOOCONFIG_H
#define PZOOCONFIG_H

#include "toml.hpp"
#include "PZtdMgr.h"
#include <QSettings>
#include <QDir>
#include "../src/PConfigMgr.h"

class PZooConfig : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString zooConfigPath READ getZooConfigPath WRITE setZooConfigPath NOTIFY zooConfigPathChanged)
    Q_PROPERTY(QString logCutoff READ getLogCutoff WRITE setLogCutoff NOTIFY logCutoffChanged)
    Q_PROPERTY(QString sendLogfile READ getSendLogfile WRITE setSendLogfile NOTIFY sendLogfileChanged)
    Q_PROPERTY(QString sendDebugger READ getSendDebugger WRITE setSendDebugger NOTIFY sendDebuggerChanged)
    Q_PROPERTY(QString deltaLog1 READ getDeltaLog1 WRITE setDeltaLog1 NOTIFY deltaLog1Changed)
    Q_PROPERTY(QString deltaLog0 READ getDeltaLog0 WRITE setDeltaLog0 NOTIFY deltaLog0Changed)
    Q_PROPERTY(QString drawfps READ getDrawfps WRITE setDrawfps NOTIFY drawfpsChanged)
    Q_PROPERTY(QString drawfpsx READ getDrawfpsx WRITE setDrawfpsx NOTIFY drawfpsxChanged)
    Q_PROPERTY(QString drawfpsy READ getDrawfpsy WRITE setDrawfpsy NOTIFY drawfpsyChanged)
    Q_PROPERTY(QString aimgr READ getAimgr WRITE setAimgr NOTIFY aimgrChanged)
    Q_PROPERTY(QString worldmgr READ getWorldmgr WRITE setWorldmgr NOTIFY worldmgrChanged)
    Q_PROPERTY(QString gamemgr READ getGamemgr WRITE setGamemgr NOTIFY gamemgrChanged)
    Q_PROPERTY(QString scenariomgr READ getScenariomgr WRITE setScenariomgr NOTIFY scenariomgrChanged)
    Q_PROPERTY(QString scriptmgr READ getScriptmgr WRITE setScriptmgr NOTIFY scriptmgrChanged)
    Q_PROPERTY(QString soundmgr READ getSoundmgr WRITE setSoundmgr NOTIFY soundmgrChanged)
    Q_PROPERTY(QString terrainmgr READ getTerrainmgr WRITE setTerrainmgr NOTIFY terrainmgrChanged)
    Q_PROPERTY(QString lang READ getLang WRITE setLang NOTIFY langChanged)
    Q_PROPERTY(QString sublang READ getSublang WRITE setSublang NOTIFY sublangChanged)
    Q_PROPERTY(QString res READ getRes WRITE setRes NOTIFY resChanged)
    Q_PROPERTY(QString langLib READ getLangLib WRITE setLangLib NOTIFY langLibChanged)
    Q_PROPERTY(QStringList path READ getPath WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(QString fullscreen READ getFullscreen WRITE setFullscreen NOTIFY fullscreenChanged)
    Q_PROPERTY(QString screenwidth READ getScreenwidth WRITE setScreenwidth NOTIFY screenwidthChanged)
    Q_PROPERTY(QString screenheight READ getScreenheight WRITE setScreenheight NOTIFY screenheightChanged)
    Q_PROPERTY(QString updateRate READ getUpdateRate WRITE setUpdateRate NOTIFY updateRateChanged)
    Q_PROPERTY(QString drawRate READ getDrawRate WRITE setDrawRate NOTIFY drawRateChanged)
    Q_PROPERTY(QString lastfile READ getLastfile WRITE setLastfile NOTIFY lastfileChanged)
    Q_PROPERTY(QString unlockCount READ getUnlockCount WRITE setUnlockCount NOTIFY unlockCountChanged)
    Q_PROPERTY(QStringList unlockEntity READ getUnlockEntity WRITE setUnlockEntity NOTIFY unlockEntityChanged)
    Q_PROPERTY(QString drag READ getDrag WRITE setDrag NOTIFY dragChanged)
    Q_PROPERTY(QString click READ getClick WRITE setClick NOTIFY clickChanged)
    Q_PROPERTY(QString normal READ getNormal WRITE setNormal NOTIFY normalChanged)
    Q_PROPERTY(QString loadHalfAnims READ getLoadHalfAnims WRITE setLoadHalfAnims NOTIFY loadHalfAnimsChanged)
    Q_PROPERTY(QString use8BitSound READ getUse8BitSound WRITE setUse8BitSound NOTIFY use8BitSoundChanged)
    Q_PROPERTY(QString level READ getLevel WRITE setLevel NOTIFY levelChanged)
    Q_PROPERTY(QString devModeEnabled READ getDevModeEnabled WRITE setDevModeEnabled NOTIFY devModeEnabledChanged)
    Q_PROPERTY(QString mapX READ getMapX WRITE setMapX NOTIFY mapXChanged)
    Q_PROPERTY(QString mapY READ getMapY WRITE setMapY NOTIFY mapYChanged)
    Q_PROPERTY(QString useAlternateCursors READ getUseAlternateCursors WRITE setUseAlternateCursors NOTIFY useAlternateCursorsChanged)
    Q_PROPERTY(QString tooltipDelay READ getTooltipDelay WRITE setTooltipDelay NOTIFY tooltipDelayChanged)
    Q_PROPERTY(QString tooltipDuration READ getTooltipDuration WRITE setTooltipDuration NOTIFY tooltipDurationChanged)
    Q_PROPERTY(QString messageDisplay READ getMessageDisplay WRITE setMessageDisplay NOTIFY messageDisplayChanged)

    Q_PROPERTY(QString mouseScrollThreshold READ getMouseScrollThreshold WRITE setMouseScrollThreshold NOTIFY mouseScrollThresholdChanged)
    Q_PROPERTY(QString mouseScrollDelay READ getMouseScrollDelay WRITE setMouseScrollDelay NOTIFY mouseScrollDelayChanged)
    Q_PROPERTY(QString mouseScrollX READ getMouseScrollX WRITE setMouseScrollX NOTIFY mouseScrollXChanged)
    Q_PROPERTY(QString mouseScrollY READ getMouseScrollY WRITE setMouseScrollY NOTIFY mouseScrollYChanged)
    Q_PROPERTY(QString keyScrollX READ getKeyScrollX WRITE setKeyScrollX NOTIFY keyScrollXChanged)
    Q_PROPERTY(QString keyScrollY READ getKeyScrollY WRITE setKeyScrollY NOTIFY keyScrollYChanged)
    Q_PROPERTY(QString minimumMessageInterval READ getMinimumMessageInterval WRITE setMinimumMessageInterval NOTIFY minimumMessageIntervalChanged)
    Q_PROPERTY(QString defaultEditCharLimit READ getDefaultEditCharLimit WRITE setDefaultEditCharLimit NOTIFY defaultEditCharLimitChanged)
    Q_PROPERTY(QString noMenuMusic READ getNoMenuMusic WRITE setNoMenuMusic NOTIFY noMenuMusicChanged)
    Q_PROPERTY(QString menuMusic READ getMenuMusic WRITE setMenuMusic NOTIFY menuMusicChanged)
    Q_PROPERTY(QString menuMusicAttenuation READ getMenuMusicAttenuation WRITE setMenuMusicAttenuation NOTIFY menuMusicAttenuationChanged)
    Q_PROPERTY(QString userAttenuation READ getUserAttenuation WRITE setUserAttenuation NOTIFY userAttenuationChanged)
    Q_PROPERTY(QString helpType READ getHelpType WRITE setHelpType NOTIFY helpTypeChanged)
    Q_PROPERTY(QString playMovie READ getPlayMovie WRITE setPlayMovie NOTIFY playMovieChanged)
    Q_PROPERTY(QString movievolume1 READ getMovievolume1 WRITE setMovievolume1 NOTIFY movievolume1Changed)
    Q_PROPERTY(QString playSecondMovie READ getPlaySecondMovie WRITE setPlaySecondMovie NOTIFY playSecondMovieChanged)
    Q_PROPERTY(QString movievolume2 READ getMovievolume2 WRITE setMovievolume2 NOTIFY movievolume2Changed)
    Q_PROPERTY(QString maxShortTooltipWidth READ getMaxShortTooltipWidth WRITE setMaxShortTooltipWidth NOTIFY maxShortTooltipWidthChanged)
    Q_PROPERTY(QString maxLongTooltipWidth READ getMaxLongTooltipWidth WRITE setMaxLongTooltipWidth NOTIFY maxLongTooltipWidthChanged)
    Q_PROPERTY(QString progressLeft READ getProgressLeft WRITE setProgressLeft NOTIFY progressLeftChanged)
    Q_PROPERTY(QString progressTop READ getProgressTop WRITE setProgressTop NOTIFY progressTopChanged)
    Q_PROPERTY(QString progressRight READ getProgressRight WRITE setProgressRight NOTIFY progressRightChanged)
    Q_PROPERTY(QString progressBottom READ getProgressBottom WRITE setProgressBottom NOTIFY progressBottomChanged)
    Q_PROPERTY(QString progressRed READ getProgressRed WRITE setProgressRed NOTIFY progressRedChanged)
    Q_PROPERTY(QString progressGreen READ getProgressGreen WRITE setProgressGreen NOTIFY progressGreenChanged)
    Q_PROPERTY(QString progressBlue READ getProgressBlue WRITE setProgressBlue NOTIFY progressBlueChanged)
    Q_PROPERTY(QString progressShadowXOffset READ getProgressShadowXOffset WRITE setProgressShadowXOffset NOTIFY progressShadowXOffsetChanged)
    Q_PROPERTY(QString progressShadowYOffset READ getProgressShadowYOffset WRITE setProgressShadowYOffset NOTIFY progressShadowYOffsetChanged)
    Q_PROPERTY(QString progressShadowRed READ getProgressShadowRed WRITE setProgressShadowRed NOTIFY progressShadowRedChanged)
    Q_PROPERTY(QString progressShadowGreen READ getProgressShadowGreen WRITE setProgressShadowGreen NOTIFY progressShadowGreenChanged)
    Q_PROPERTY(QString progressShadowBlue READ getProgressShadowBlue WRITE setProgressShadowBlue NOTIFY progressShadowBlueChanged)
    Q_PROPERTY(QString completedExhibitAttenuation READ getCompletedExhibitAttenuation WRITE setCompletedExhibitAttenuation NOTIFY completedExhibitAttenuationChanged)
    Q_PROPERTY(QString MSStartingCash READ getMSStartingCash WRITE setMSStartingCash NOTIFY MSStartingCashChanged)
    Q_PROPERTY(QString MSCashIncrement READ getMSCashIncrement WRITE setMSCashIncrement NOTIFY MSCashIncrementChanged)
    Q_PROPERTY(QString MSMinCash READ getMSMinCash WRITE setMSMinCash NOTIFY MSMinCashChanged)
    Q_PROPERTY(QString MSMaxCash READ getMSMaxCash WRITE setMSMaxCash NOTIFY MSMaxCashChanged)
    Q_PROPERTY(QString startedFirstTutorial READ getStartedFirstTutorial WRITE setStartedFirstTutorial NOTIFY startedFirstTutorialChanged)
    Q_PROPERTY(QString progresscalls READ getProgresscalls WRITE setProgresscalls NOTIFY progresscallsChanged)
    Q_PROPERTY(QString startedDinoTutorial READ getStartedDinoTutorial WRITE setStartedDinoTutorial NOTIFY startedDinoTutorialChanged)
    Q_PROPERTY(QString startedAquaTutorial READ getStartedAquaTutorial WRITE setStartedAquaTutorial NOTIFY startedAquaTutorialChanged)
    Q_PROPERTY(QString lastWindowX READ getLastWindowX WRITE setLastWindowX NOTIFY lastWindowXChanged)
    Q_PROPERTY(QString lastWindowY READ getLastWindowY WRITE setLastWindowY NOTIFY lastWindowYChanged)
    Q_PROPERTY(QString tutorial READ getTutorial WRITE setTutorial NOTIFY tutorialChanged)
    Q_PROPERTY(QString aa READ getAa WRITE setAa NOTIFY aaChanged)
    Q_PROPERTY(QString ab READ getAb WRITE setAb NOTIFY abChanged)
    Q_PROPERTY(QString ac READ getAc WRITE setAc NOTIFY acChanged)
    Q_PROPERTY(QString ad READ getAd WRITE setAd NOTIFY adChanged)
    Q_PROPERTY(QString ae READ getAe WRITE setAe NOTIFY aeChanged)
    Q_PROPERTY(QString af READ getAf WRITE setAf NOTIFY afChanged)
    Q_PROPERTY(QString ag READ getAg WRITE setAg NOTIFY agChanged)
    Q_PROPERTY(QString ba READ getBa WRITE setBa NOTIFY baChanged)
    Q_PROPERTY(QString bb READ getBb WRITE setBb NOTIFY bbChanged)
    Q_PROPERTY(QString bc READ getBc WRITE setBc NOTIFY bcChanged)
    Q_PROPERTY(QString bd READ getBd WRITE setBd NOTIFY bdChanged)
    Q_PROPERTY(QString be READ getBe WRITE setBe NOTIFY beChanged)
    Q_PROPERTY(QString bf READ getBf WRITE setBf NOTIFY bfChanged)
    Q_PROPERTY(QString bg READ getBg WRITE setBg NOTIFY bgChanged)
    Q_PROPERTY(QString bh READ getBh WRITE setBh NOTIFY bhChanged)
    Q_PROPERTY(QString bi READ getBi WRITE setBi NOTIFY biChanged)
    Q_PROPERTY(QString ca READ getCa WRITE setCa NOTIFY caChanged)
    Q_PROPERTY(QString cb READ getCb WRITE setCb NOTIFY cbChanged)
    Q_PROPERTY(QString cc READ getCc WRITE setCc NOTIFY ccChanged)
    Q_PROPERTY(QString cd READ getCd WRITE setCd NOTIFY cdChanged)
    Q_PROPERTY(QString ce READ getCe WRITE setCe NOTIFY ceChanged)
    Q_PROPERTY(QString cf READ getCf WRITE setCf NOTIFY cfChanged)
    Q_PROPERTY(QString cg READ getCg WRITE setCg NOTIFY cgChanged)
    Q_PROPERTY(QString ch READ getCh WRITE setCh NOTIFY chChanged)
    Q_PROPERTY(QString ci READ getCi WRITE setCi NOTIFY ciChanged)
    Q_PROPERTY(QString cj READ getCj WRITE setCj NOTIFY cjChanged)
    Q_PROPERTY(QString ck READ getCk WRITE setCk NOTIFY ckChanged)
    Q_PROPERTY(QString cl READ getCl WRITE setCl NOTIFY clChanged)
    Q_PROPERTY(QString cm READ getCm WRITE setCm NOTIFY cmChanged)
    Q_PROPERTY(QString cn READ getCn WRITE setCn NOTIFY cnChanged)
    Q_PROPERTY(QString da READ getDa WRITE setDa NOTIFY daChanged)
    Q_PROPERTY(QString db READ getDb WRITE setDb NOTIFY dbChanged)
    Q_PROPERTY(QString dc READ getDc WRITE setDc NOTIFY dcChanged)
    Q_PROPERTY(QString dd READ getDd WRITE setDd NOTIFY ddChanged)
    Q_PROPERTY(QString de READ getDe WRITE setDe NOTIFY deChanged)
    Q_PROPERTY(QString df READ getDf WRITE setDf NOTIFY dfChanged)
    Q_PROPERTY(QString dg READ getDg WRITE setDg NOTIFY dgChanged)
    Q_PROPERTY(QString dh READ getDh WRITE setDh NOTIFY dhChanged)
    Q_PROPERTY(QString ea READ getEa WRITE setEa NOTIFY eaChanged)
    Q_PROPERTY(QString fa READ getFa WRITE setFa NOTIFY faChanged)
    Q_PROPERTY(QString fb READ getFb WRITE setFb NOTIFY fbChanged)
    Q_PROPERTY(QString ga READ getGa WRITE setGa NOTIFY gaChanged)
    Q_PROPERTY(QString gb READ getGb WRITE setGb NOTIFY gbChanged)
    Q_PROPERTY(QString gc READ getGc WRITE setGc NOTIFY gcChanged)
    Q_PROPERTY(QString gd READ getGd WRITE setGd NOTIFY gdChanged)


public:
    explicit PZooConfig(QObject *parent = nullptr, QString zooConfigPath = "");
    PZooConfig(const PZooConfig&) = delete;
    PZooConfig& operator=(const PZooConfig&) = delete;
    PZooConfig(PZooConfig&&) = default;

    QString getZooConfigPath() const { return m_zooConfigPath; }
    void setZooConfigPath(const QString &path);

    // ---------------------------------------------------- [debug]

    QString getLogCutoff() const { return m_logCutoff; }
    void setLogCutoff(const QString &logCutoff);
    QString getSendLogfile() const { return m_sendLogfile; }
    void setSendLogfile(const QString &sendLogfile);
    QString getSendDebugger() const { return m_sendDebugger; }
    void setSendDebugger(const QString &sendDebugger);
    QString getDeltaLog1() const { return m_deltaLog1; }
    void setDeltaLog1(const QString &deltaLog1);
    QString getDeltaLog0() const { return m_deltaLog0; }
    void setDeltaLog0(const QString &deltaLog0);
    QString getDrawfps() const { return m_drawfps; }
    void setDrawfps(const QString &drawfps);
    QString getDrawfpsx() const { return m_drawfpsx; }
    void setDrawfpsx(const QString &drawfpsx);
    QString getDrawfpsy() const { return m_drawfpsy; }
    void setDrawfpsy(const QString &drawfpsy);

    // ---------------------------------------------------- [mgr]

    QString getAimgr() const { return m_aimgr; }
    void setAimgr(const QString &aimgr);
    QString getWorldmgr() const { return m_worldmgr; }
    void setWorldmgr(const QString &worldmgr);
    QString getGamemgr() const { return m_gamemgr; }
    void setGamemgr(const QString &gamemgr);
    QString getScenariomgr() const { return m_scenariomgr; }
    void setScenariomgr(const QString &scenariomgr);
    QString getScriptmgr() const { return m_scriptmgr; }
    void setScriptmgr(const QString &scriptmgr);
    QString getSoundmgr() const { return m_soundmgr; }
    void setSoundmgr(const QString &soundmgr);
    QString getTerrainmgr() const { return m_terrainmgr; }
    void setTerrainmgr(const QString &terrainmgr);

    // ---------------------------------------------------- [language]

    QString getLang() const { return m_lang; }
    void setLang(const QString &lang);
    QString getSublang() const { return m_sublang; }
    void setSublang(const QString &sublang);
    
    // ---------------------------------------------------- [lib]

    QString getRes() const { return m_res; }
    void setRes(const QString &res);
    QString getLangLib() const { return m_langLib; }
    void setLangLib(const QString &langLib);

    // ---------------------------------------------------- [resource]

    QStringList getPath() const { return m_path; }
    void setPath(const QStringList &path);

    // ---------------------------------------------------- [user]

    QString getFullscreen() const { return m_fullscreen; }
    void setFullscreen(const QString &fullscreen);
    QString getScreenwidth() const { return m_screenwidth; }
    void setScreenwidth(const QString &screenwidth);
    QString getScreenheight() const { return m_screenheight; }
    void setScreenheight(const QString &screenheight);
    QString getUpdateRate() const { return m_UpdateRate; }
    void setUpdateRate(const QString &updateRate);
    QString getDrawRate() const { return m_DrawRate; }
    void setDrawRate(const QString &drawRate);
    QString getLastfile() const { return m_lastfile; }
    void setLastfile(const QString &lastfile);
    QString getUnlockCount() const { return m_unlockCount; }
    void setUnlockCount(const QString &unlockCount);
    QStringList getUnlockEntity() const { return m_unlockEntity; }
    void setUnlockEntity(const QStringList &unlockEntity);
    void addUnlockEntity(const QString &unlockEntity);
    void removeUnlockEntity(const QString &unlockEntity);

    // ---------------------------------------------------- [advanced]

    QString getDrag() const { return m_drag; }
    void setDrag(const QString &drag);
    QString getClick() const { return m_click; }
    void setClick(const QString &click);
    QString getNormal() const { return m_normal; }
    void setNormal(const QString &normal);
    QString getLoadHalfAnims() const { return m_loadHalfAnims; }
    void setLoadHalfAnims(const QString &loadHalfAnims);
    QString getUse8BitSound() const { return m_use8BitSound; }
    void setUse8BitSound(const QString &use8BitSound);
    QString getLevel() const { return m_level; }
    void setLevel(const QString &level);
    QString getDevModeEnabled() const { return m_devModeEnabled; }
    void setDevModeEnabled(const QString &devModeEnabled);

    // ---------------------------------------------------- [Map]

    QString getMapX() const { return m_mapX; }
    void setMapX(const QString &mapX);
    QString getMapY() const { return m_mapY; }
    void setMapY(const QString &mapY);

    // ---------------------------------------------------- [UI]

    QString getUseAlternateCursors() const { return m_useAlternateCursors; }
    void setUseAlternateCursors(const QString &useAlternateCursors);
    QString getTooltipDelay() const { return m_tooltipDelay; }
    void setTooltipDelay(const QString &tooltipDelay);
    QString getTooltipDuration() const { return m_tooltipDuration; }
    void setTooltipDuration(const QString &tooltipDuration);
    QString getMessageDisplay() const { return m_MessageDisplay; }
    void setMessageDisplay(const QString &messageDisplay);
    QString getMouseScrollThreshold() const { return m_mouseScrollThreshold; }
    void setMouseScrollThreshold(const QString &mouseScrollThreshold);
    QString getMouseScrollDelay() const { return m_mouseScrollDelay; }
    void setMouseScrollDelay(const QString &mouseScrollDelay);
    QString getMouseScrollX() const { return m_mouseScrollX; }
    void setMouseScrollX(const QString &mouseScrollX);
    QString getMouseScrollY() const { return m_mouseScrollY; }
    void setMouseScrollY(const QString &mouseScrollY);
    QString getKeyScrollX() const { return m_keyScrollX; }
    void setKeyScrollX(const QString &keyScrollX);
    QString getKeyScrollY() const { return m_keyScrollY; }
    void setKeyScrollY(const QString &keyScrollY);
    QString getMinimumMessageInterval() const { return m_minimumMessageInterval; }
    void setMinimumMessageInterval(const QString &minimumMessageInterval);
    QString getDefaultEditCharLimit() const { return m_defaultEditCharLimit; }
    void setDefaultEditCharLimit(const QString &defaultEditCharLimit);
    QString getNoMenuMusic() const { return m_noMenuMusic; }
    void setNoMenuMusic(const QString &noMenuMusic);
    QString getMenuMusic() const { return m_menuMusic; }
    void setMenuMusic(const QString &menuMusic);
    QString getMenuMusicAttenuation() const { return m_menuMusicAttenuation; }
    void setMenuMusicAttenuation(const QString &menuMusicAttenuation);
    QString getUserAttenuation() const { return m_userAttenuation; }
    void setUserAttenuation(const QString &userAttenuation);
    QString getHelpType() const { return m_helpType; }
    void setHelpType(const QString &helpType);
    QString getPlayMovie() const { return m_playMovie; }
    void setPlayMovie(const QString &playMovie);
    QString getMovievolume1() const { return m_movievolume1; }
    void setMovievolume1(const QString &movievolume1);
    QString getPlaySecondMovie() const { return m_playSecondMovie; }
    void setPlaySecondMovie(const QString &playSecondMovie);
    QString getMovievolume2() const { return m_movievolume2; }
    void setMovievolume2(const QString &movievolume2);
    QString getMaxShortTooltipWidth() const { return m_maxShortTooltipWidth; }
    void setMaxShortTooltipWidth(const QString &maxShortTooltipWidth);
    QString getMaxLongTooltipWidth() const { return m_maxLongTooltipWidth; }
    void setMaxLongTooltipWidth(const QString &maxLongTooltipWidth);
    QString getProgressLeft() const { return m_progressLeft; }
    void setProgressLeft(const QString &progressLeft);
    QString getProgressTop() const { return m_progressTop; }
    void setProgressTop(const QString &progressTop);
    QString getProgressRight() const { return m_progressRight; }
    void setProgressRight(const QString &progressRight);
    QString getProgressBottom() const { return m_progressBottom; }
    void setProgressBottom(const QString &progressBottom);
    QString getProgressRed() const { return m_progressRed; }
    void setProgressRed(const QString &progressRed);
    QString getProgressGreen() const { return m_progressGreen; }
    void setProgressGreen(const QString &progressGreen);
    QString getProgressBlue() const { return m_progressBlue; }
    void setProgressBlue(const QString &progressBlue);
    QString getProgressShadowXOffset() const { return m_progressShadowXOffset; }
    void setProgressShadowXOffset(const QString &progressShadowXOffset);
    QString getProgressShadowYOffset() const { return m_progressShadowYOffset; }
    void setProgressShadowYOffset(const QString &progressShadowYOffset);
    QString getProgressShadowRed() const { return m_progressShadowRed; }
    void setProgressShadowRed(const QString &progressShadowRed);
    QString getProgressShadowGreen() const { return m_progressShadowGreen; }
    void setProgressShadowGreen(const QString &progressShadowGreen);
    QString getProgressShadowBlue() const { return m_progressShadowBlue; }
    void setProgressShadowBlue(const QString &progressShadowBlue);
    QString getCompletedExhibitAttenuation() const { return m_completedExhibitAttenuation; }
    void setCompletedExhibitAttenuation(const QString &completedExhibitAttenuation);
    QString getMSStartingCash() const { return m_MSStartingCash; }
    void setMSStartingCash(const QString &MSStartingCash);
    QString getMSCashIncrement() const { return m_MSCashIncrement; }
    void setMSCashIncrement(const QString &MSCashIncrement);
    QString getMSMinCash() const { return m_MSMinCash; }
    void setMSMinCash(const QString &MSMinCash);
    QString getMSMaxCash() const { return m_MSMaxCash; }
    void setMSMaxCash(const QString &MSMaxCash);
    QString getStartedFirstTutorial() const { return m_startedFirstTutorial; }
    void setStartedFirstTutorial(const QString &startedFirstTutorial);
    QString getProgresscalls() const { return m_progresscalls; }
    void setProgresscalls(const QString &progresscalls);
    QString getStartedDinoTutorial() const { return m_startedDinoTutorial; }
    void setStartedDinoTutorial(const QString &startedDinoTutorial);
    QString getStartedAquaTutorial() const { return m_startedAquaTutorial; }
    void setStartedAquaTutorial(const QString &startedAquaTutorial);
    QString getLastWindowX() const { return m_lastWindowX; }
    void setLastWindowX(const QString &lastWindowX);
    QString getLastWindowY() const { return m_lastWindowY; }
    void setLastWindowY(const QString &lastWindowY);

    // ---------------------------------------------------- [scenario]
    QString getTutorial() const { return m_tutorial; }
    void setTutorial(const QString &tutorial);
    QString getAa() const { return m_aa; }
    void setAa(const QString &aa);
    QString getAb() const { return m_ab; }
    void setAb(const QString &ab);
    QString getAc() const { return m_ac; }
    void setAc(const QString &ac);
    QString getAd() const { return m_ad; }
    void setAd(const QString &ad);
    QString getAe() const { return m_ae; }
    void setAe(const QString &ae);
    QString getAf() const { return m_af; }
    void setAf(const QString &af);
    QString getAg() const { return m_ag; }
    void setAg(const QString &ag);
    QString getBa() const { return m_ba; }
    void setBa(const QString &ba);
    QString getBb() const { return m_bb; }
    void setBb(const QString &bb);
    QString getBc() const { return m_bc; }
    void setBc(const QString &bc);
    QString getBd() const { return m_bd; }
    void setBd(const QString &bd);
    QString getBe() const { return m_be; }
    void setBe(const QString &be);
    QString getBf() const { return m_bf; }
    void setBf(const QString &bf);
    QString getBg() const { return m_bg; }
    void setBg(const QString &bg);
    QString getBh() const { return m_bh; }
    void setBh(const QString &bh);
    QString getBi() const { return m_bi; }
    void setBi(const QString &bi);
    QString getCa() const { return m_ca; }
    void setCa(const QString &ca);
    QString getCb() const { return m_cb; }
    void setCb(const QString &cb);
    QString getCc() const { return m_cc; }
    void setCc(const QString &cc);
    QString getCd() const { return m_cd; }
    void setCd(const QString &cd);
    QString getCe() const { return m_ce; }
    void setCe(const QString &ce);
    QString getCf() const { return m_cf; }
    void setCf(const QString &cf);
    QString getCg() const { return m_cg; }
    void setCg(const QString &cg);
    QString getCh() const { return m_ch; }
    void setCh(const QString &ch);
    QString getCi() const { return m_ci; }
    void setCi(const QString &ci);
    QString getCj() const { return m_cj; }
    void setCj(const QString &cj);
    QString getCk() const { return m_ck; }
    void setCk(const QString &ck);
    QString getCl() const { return m_cl; }
    void setCl(const QString &cl);
    QString getCm() const { return m_cm; }
    void setCm(const QString &cm);
    QString getCn() const { return m_cn; }
    void setCn(const QString &cn);
    QString getDa() const { return m_da; }
    void setDa(const QString &da);
    QString getDb() const { return m_db; }
    void setDb(const QString &db);
    QString getDc() const { return m_dc; }
    void setDc(const QString &dc);
    QString getDd() const { return m_dd; }
    void setDd(const QString &dd);
    QString getDe() const { return m_de; }
    void setDe(const QString &de);
    QString getDf() const { return m_df; }
    void setDf(const QString &df);
    QString getDg() const { return m_dg; }
    void setDg(const QString &dg);
    QString getDh() const { return m_dh; }
    void setDh(const QString &dh);
    QString getEa() const { return m_ea; }
    void setEa(const QString &ea);
    QString getFa() const { return m_fa; }
    void setFa(const QString &fa);
    QString getFb() const { return m_fb; }
    void setFb(const QString &fb);
    QString getGa() const { return m_ga; }
    void setGa(const QString &ga);
    QString getGb() const { return m_gb; }
    void setGb(const QString &gb);
    QString getGc() const { return m_gc; }
    void setGc(const QString &gc);
    QString getGd() const { return m_gd; }
    void setGd(const QString &gd);

    // ---------------------------------------------------- [ai]
    QString getShowAIInfo() const { return m_showAIInfo; }
    void setShowAIInfo(const QString &showAIInfo);
    QString getShowName() const { return m_showName; }
    void setShowName(const QString &showName);
    QString getShowPosition() const { return m_showPosition; }
    void setShowPosition(const QString &showPosition);
    QString getShowAnimalAIInfo() const { return m_showAnimalAIInfo; }
    void setShowAnimalAIInfo(const QString &showAnimalAIInfo);
    QString getShowGuestAIInfo() const { return m_showGuestAIInfo; }
    void setShowGuestAIInfo(const QString &showGuestAIInfo);
    QString getShowStaffAIInfo() const { return m_showStaffAIInfo; }
    void setShowStaffAIInfo(const QString &showStaffAIInfo);
    QString getShowStatusVars() const { return m_showStatusVars; }
    void setShowStatusVars(const QString &showStatusVars);
    QString getShowFunctionCall() const { return m_showFunctionCall; }
    void setShowFunctionCall(const QString &showFunctionCall);
    QString getShowEvents() const { return m_showEvents; }
    void setShowEvents(const QString &showEvents);
    QString getShowBuildingAIInfo() const { return m_showBuildingAIInfo; }
    void setShowBuildingAIInfo(const QString &showBuildingAIInfo);

    Q_INVOKABLE PZooConfig defaultConfig();
    Q_INVOKABLE void updateTable(const QString &section, const QString &key, const QString &value);
    Q_INVOKABLE void updateUnlockEntity(const QString &key, const QString &value);
    Q_INVOKABLE void saveConfig();
    Q_INVOKABLE void loadConfig();
    Q_INVOKABLE void revertChanges();

signals:
    void configUpdated(const QString &section, const QString &key, const QString &value);
    void configLoaded(const QString &configPath);
    void configSaved(const QString &configPath);
    void configError(const QString &errorMessage);
    void logCutoffChanged(const QString &logCutoff);
    void sendLogfileChanged(const QString &sendLogfile);
    void sendDebuggerChanged(const QString &sendDebugger);
    void deltaLog1Changed(const QString &deltaLog1);
    void deltaLog0Changed(const QString &deltaLog0);
    void drawfpsChanged(const QString &drawfps);
    void drawfpsxChanged(const QString &drawfpsx);
    void drawfpsyChanged(const QString &drawfpsy);
    void aimgrChanged(const QString &aimgr);
    void worldmgrChanged(const QString &worldmgr);
    void gamemgrChanged(const QString &gamemgr);
    void scenariomgrChanged(const QString &scenariomgr);
    void scriptmgrChanged(const QString &scriptmgr);
    void soundmgrChanged(const QString &soundmgr);
    void terrainmgrChanged(const QString &terrainmgr);

    void langChanged(const QString &lang);
    void sublangChanged(const QString &sublang);
    void resChanged(const QString &res);
    void langLibChanged(const QString &langLib);
    void pathChanged(const QStringList &path);
    void fullscreenChanged(const QString &fullscreen);
    void screenwidthChanged(const QString &screenwidth);
    void screenheightChanged(const QString &screenheight);
    void updateRateChanged(const QString &updateRate);
    void drawRateChanged(const QString &drawRate);
    void lastfileChanged(const QString &lastfile);
    void unlockCountChanged(const QString &unlockCount);
    void unlockEntityChanged(const QStringList &unlockEntity);
    void dragChanged(const QString &drag);
    void clickChanged(const QString &click);
    void normalChanged(const QString &normal);
    void loadHalfAnimsChanged(const QString &loadHalfAnims);
    void use8BitSoundChanged(const QString &use8BitSound);
    void levelChanged(const QString &level);
    void devModeEnabledChanged(const QString &devModeEnabled);
    void mapXChanged(const QString &mapX);
    void mapYChanged(const QString &mapY);
    void useAlternateCursorsChanged(const QString &useAlternateCursors);
    void tooltipDelayChanged(const QString &tooltipDelay);
    void tooltipDurationChanged(const QString &tooltipDuration);
    void messageDisplayChanged(const QString &messageDisplay);
    void mouseScrollThresholdChanged(const QString &mouseScrollThreshold);
    void mouseScrollDelayChanged(const QString &mouseScrollDelay);
    void mouseScrollXChanged(const QString &mouseScrollX);
    void mouseScrollYChanged(const QString &mouseScrollY);
    void keyScrollXChanged(const QString &keyScrollX);
    void keyScrollYChanged(const QString &keyScrollY);
    void minimumMessageIntervalChanged(const QString &minimumMessageInterval);
    void defaultEditCharLimitChanged(const QString &defaultEditCharLimit);
    void noMenuMusicChanged(const QString &noMenuMusic);
    void menuMusicChanged(const QString &menuMusic);
    void menuMusicAttenuationChanged(const QString &menuMusicAttenuation);
    void userAttenuationChanged(const QString &userAttenuation);
    void helpTypeChanged(const QString &helpType);
    void playMovieChanged(const QString &playMovie);
    void movievolume1Changed(const QString &movievolume1);
    void playSecondMovieChanged(const QString &playSecondMovie);
    void movievolume2Changed(const QString &movievolume2);
    void maxShortTooltipWidthChanged(const QString &maxShortTooltipWidth);
    void maxLongTooltipWidthChanged(const QString &maxLongTooltipWidth);
    void progressLeftChanged(const QString &progressLeft);

    void progressTopChanged(const QString &progressTop);
    void progressRightChanged(const QString &progressRight);
    void progressBottomChanged(const QString &progressBottom);
    void progressRedChanged(const QString &progressRed);
    void progressGreenChanged(const QString &progressGreen);
    void progressBlueChanged(const QString &progressBlue);
    void progressShadowXOffsetChanged(const QString &progressShadowXOffset);
    void progressShadowYOffsetChanged(const QString &progressShadowYOffset);
    void progressShadowRedChanged(const QString &progressShadowRed);
    void progressShadowGreenChanged(const QString &progressShadowGreen);
    void progressShadowBlueChanged(const QString &progressShadowBlue);
    void completedExhibitAttenuationChanged(const QString &completedExhibitAttenuation);
    void MSStartingCashChanged(const QString &MSStartingCash);
    void MSCashIncrementChanged(const QString &MSCashIncrement);
    void MSMinCashChanged(const QString &MSMinCash);
    void MSMaxCashChanged(const QString &MSMaxCash);
    void startedFirstTutorialChanged(const QString &startedFirstTutorial);
    void progresscallsChanged(const QString &progresscalls);
    void startedDinoTutorialChanged(const QString &startedDinoTutorial);
    void startedAquaTutorialChanged(const QString &startedAquaTutorial);
    void lastWindowXChanged(const QString &lastWindowX);
    void lastWindowYChanged(const QString &lastWindowY);
    void tutorialChanged(const QString &tutorial);
    void aaChanged(const QString &aa);
    void abChanged(const QString &ab);
    void acChanged(const QString &ac);
    void adChanged(const QString &ad);
    void aeChanged(const QString &ae);
    void afChanged(const QString &af);
    void agChanged(const QString &ag);
    void baChanged(const QString &ba);
    void bbChanged(const QString &bb);
    void bcChanged(const QString &bc);
    void bdChanged(const QString &bd);
    void beChanged(const QString &be);
    void bfChanged(const QString &bf);
    void bgChanged(const QString &bg);
    void bhChanged(const QString &bh);
    void biChanged(const QString &bi);
    void caChanged(const QString &ca);
    void cbChanged(const QString &cb);
    void ccChanged(const QString &cc);
    void cdChanged(const QString &cd);
    void ceChanged(const QString &ce);
    void cfChanged(const QString &cf);
    void cgChanged(const QString &cg);
    void chChanged(const QString &ch);
    void ciChanged(const QString &ci);
    void cjChanged(const QString &cj);
    void ckChanged(const QString &ck);
    void clChanged(const QString &cl);
    void cmChanged(const QString &cm);
    void cnChanged(const QString &cn);
    void daChanged(const QString &da);
    void dbChanged(const QString &db);
    void dcChanged(const QString &dc);
    void ddChanged(const QString &dd);
    void deChanged(const QString &de);
    void dfChanged(const QString &df);
    void dgChanged(const QString &dg);
    void dhChanged(const QString &dh);
    void eaChanged(const QString &ea);
    void faChanged(const QString &fa);
    void fbChanged(const QString &fb);
    void gaChanged(const QString &ga);
    void gbChanged(const QString &gb);
    void gcChanged(const QString &gc);
    void gdChanged(const QString &gd);
private:
    QString m_zooConfigPath;
    toml::table m_configTable;
    bool m_dirty;
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
    QStringList m_path;

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