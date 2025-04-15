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

    Q_INVOKABLE void PZooConfig resetZooConfig();
private:
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