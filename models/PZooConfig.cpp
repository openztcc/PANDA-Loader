#include "PZooConfig.h"

PZooConfig::PZooConfig(QObject *parent, QString zooConfigPath) : QObject(parent), m_zooConfigPath(zooConfigPath) {
    if (zooConfigPath.isEmpty()) {
        m_zooConfigPath = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\zoo.ini";
    } else {
        m_zooConfigPath = zooConfigPath;
    }

    // Initialize the config table with default values
    loadConfig();

    m_dirty = 0;
    dirtyList.clear();
    emit dirtyChanged(m_dirty);
}

// TODO: Re-implement with QMap
// for when user wants the default settings
// std::unique_ptr<QBuffer> PZooConfig::defaultConfig() {
//     // create a new buffer for the default config
//     std::unique_ptr<QBuffer> configBuffer = std::make_unique<QBuffer>();
//     configBuffer->open(QIODevice::ReadWrite);
//     QSettings config(static_cast<QIODevice*>(configBuffer.get()), QSettings::IniFormat);

//     // [debug]
//     config.beginGroup("debug");
//     config.setValue("logCutoff", 15);
//     config.setValue("sendLogfile", false);
//     config.setValue("sendDebugger", true);
//     config.setValue("deltaLog1", false);
//     config.setValue("deltaLog0", false);
//     config.setValue("drawfps", false);
//     config.setValue("drawfpsx", 720);
//     config.setValue("drawfpsy", 20);
//     config.endGroup();

//     // [mgr]
//     config.beginGroup("mgr");
//     config.setValue("aimgr", "ZTAIMgr");
//     config.setValue("worldmgr", "ZTWorldMgr");
//     config.setValue("gamemgr", "BFGameMgr");
//     config.setValue("scenariomgr", "ZTScenarioMgr");
//     config.setValue("scriptmgr", "BFScriptMgr");
//     config.setValue("soundmgr", "BFSoundMgr");
//     config.setValue("terrainmgr", "ZTAdvTerrainMgr");
//     config.endGroup();

//     // [language]
//     config.beginGroup("language");
//     config.setValue("lang", 9);
//     config.setValue("sublang", 1);
//     config.endGroup();

//     // [lib]
//     config.beginGroup("lib");
//     config.setValue("res", "res0.dll");
//     config.setValue("langLib", "lang0.dll");
//     config.setValue("res1", "res1.dll");
//     config.endGroup();

//     // [resource]
//     config.beginGroup("resource");
//     config.setValue("path", "./dlupdate;./updates;./xpack2/loc;./xpack2;./dupdate;./zupdate1;./xpack1/loc;./xpack1;./zupdate;./loc;.");
//     config.endGroup();

//     // [user]
//     config.beginGroup("user");
//     config.setValue("fullscreen", false);
//     config.setValue("screenwidth", 800);
//     config.setValue("screenheight", 600);
//     config.setValue("UpdateRate", 60);
//     config.setValue("DrawRate", 60);
//     config.setValue("lastfile", "");
//     config.endGroup();
//     // config.m_unlockCount = "0"; // not in ini by default
//     // config.m_unlockEntity = QStringList(); // not in ini by default

//     // [advanced]
//     config.beginGroup("advanced");
//     config.setValue("drag", 3);
//     config.setValue("click", 1);
//     config.setValue("normal", 2);
//     config.setValue("loadHalfAnims", false);
//     config.setValue("use8BitSound", false);
//     config.setValue("level", 2);
//     // config.setValue("devModeEnabled", false); // not in ini by default
//     config.endGroup();

//     // [Map]
//     config.beginGroup("Map");
//     config.setValue("mapX", 75);
//     config.setValue("mapY", 75);
//     config.endGroup();

//     // [UI]
//     config.beginGroup("UI");
//     config.setValue("useAlternateCursors", false);
//     config.setValue("tooltipDelay", true);
//     config.setValue("tooltipDuration", 3000);
//     config.setValue("MessageDisplay", true);
//     config.setValue("mouseScrollThreshold", 1);
//     config.setValue("mouseScrollDelay", true);
//     config.setValue("mouseScrollX", 69);
//     config.setValue("mouseScrollY", 69);
//     config.setValue("keyScrollX", 64);
//     config.setValue("keyScrollY", 64);
//     config.setValue("minimumMessageInterval", 60);
//     config.setValue("defaultEditCharLimit", 22);
//     config.setValue("noMenuMusic", false);
//     config.setValue("menuMusic", "sounds/mainmenu.wav");
//     config.setValue("menuMusicAttenuation", -1000);
//     config.setValue("userAttenuation", 0);
//     config.setValue("helpType", 1);
//     config.setValue("playMovie", true);
//     config.setValue("movievolume1", -1000);
//     config.setValue("playSecondMovie", true);
//     config.setValue("movievolume2", -1000);
//     config.setValue("maxShortTooltipWidth", 200);
//     config.setValue("maxLongTooltipWidth", 400);
//     config.setValue("progressLeft", 231);
//     config.setValue("progressTop", 420);
//     config.setValue("progressRight", 581);
//     config.setValue("progressBottom", 435);
//     config.setValue("progressRed", 255);
//     config.setValue("progressGreen", 184);
//     config.setValue("progressBlue", 43);
//     config.setValue("progressShadowXOffset", -3);
//     config.setValue("progressShadowYOffset", 3);
//     config.setValue("progressShadowRed", 32);
//     config.setValue("progressShadowGreen", 32);
//     config.setValue("progressShadowBlue", 32);
//     config.setValue("completedExhibitAttenuation", 100);
//     config.setValue("MSStartingCash", 75000);
//     config.setValue("MSCashIncrement", 5000);
//     config.setValue("MSMinCash", 10000);
//     config.setValue("MSMaxCash", 500000);
//     config.setValue("startedFirstTutorial", true);
//     config.setValue("progresscalls", 1280);
//     config.setValue("startedDinoTutorial", true);
//     config.setValue("startedAquaTutorial", true);
//     config.setValue("lastWindowX", 522);
//     config.setValue("lastWindowY", 86);
//     config.endGroup();

//     // [scenario] // technically default values but not in ini by default
//     // config.m_tutorial = "0";
//     // config.m_aa = "0";
//     // config.m_ab = "0";
//     // config.m_ac = "0";
//     // config.m_ad = "0";
//     // config.m_ae = "0";
//     // config.m_af = "0";
//     // config.m_ag = "0";
//     // config.m_ba = "0";
//     // config.m_bb = "0";
//     // config.m_bc = "0";
//     // config.m_bd = "0";
//     // config.m_be = "0";
//     // config.m_bf = "0";
//     // config.m_bg = "0";
//     // config.m_bh = "0";
//     // config.m_bi = "0";
//     // config.m_ca = "0";
//     // config.m_cb = "0";
//     // config.m_cc = "0";
//     // config.m_cd = "0";
//     // config.m_ce = "0";
//     // config.m_cf = "0";
//     // config.m_cg = "0";
//     // config.m_ch = "0";
//     // config.m_ci = "0";
//     // config.m_cj = "0";
//     // config.m_ck = "0";
//     // config.m_cl = "0";
//     // config.m_cm = "0";
//     // config.m_cn = "0";
//     // config.m_da = "0";
//     // config.m_db = "0";
//     // config.m_dc = "0";
//     // config.m_dd = "0";
//     // config.m_de = "0";
//     // config.m_df = "0";
//     // config.m_dg = "0";
//     // config.m_dh = "0";
//     // config.m_ea = "0";
//     // config.m_fa = "0";
//     // config.m_fb = "0";
//     // config.m_ga = "0";
//     // config.m_gb = "0";
//     // config.m_gc = "0";
//     // config.m_gd = "0";

//     // [ai] // also default values but not in ini by default
//     // config.m_showAIInfo = "0";
//     // config.m_showName = "0";
//     // config.m_showPosition = "0";
//     // config.m_showAnimalAIInfo = "0";
//     // config.m_showGuestAIInfo = "0";
//     // config.m_showStaffAIInfo = "0";
//     // config.m_showStatusVars = "0";
//     // config.m_showFunctionCall = "0";
//     // config.m_showEvents = "0";
//     // config.m_showBuildingAIInfo = "0";
//     // config.m_showSelected = "0";
//     // config.m_showFrame = "0";
//     // config.m_showGoal = "0";

//     return configBuffer;
// }

void PZooConfig::updateTable(const QString &section, const QString &key, const QString &value) {
    if (value == "true" || value == "false") {
        updateBoolTable(section, key, strToBool(value));
    } else {
        updateStrTable(section, key, value);
    }
}

bool PZooConfig::strToBool(const QString &test) {
    if (test == "1" || test == "true" || test == "True") {
        return true;
    } else if (test == "0" || test == "false" || test == "True") {
        return false;
    } else {
        return false;
    }
}

void PZooConfig::updateStrTable(const QString &section, const QString &key, const QString &value) {
    // QString input = getString(section, key);
    QString original = getString(section, key, m_zooBackup);

    qDebug() << "Original value is: " << original;
    qDebug() << "Current input to compare: " << value;
    qDebug() << "Current value of m_dirty: " << m_dirty;

    if (value == original) {
        if (m_dirty > 0) {
            if (dirtyList.contains(original)) {
                dirtyList.removeOne(original);
                m_dirty--;
                emit dirtyChanged(m_dirty);
            }
        }
        return;
    }

    m_zooini->SetValue(section.toStdString().c_str(), key.toStdString().c_str(), value.toStdString().c_str());
    if (!dirtyList.contains(original)) {
        dirtyList.append(original);
        m_dirty++;
        emit dirtyChanged(m_dirty);
    }
    emit configUpdated(section, key, value);
}

void PZooConfig::updateBoolTable(const QString &path, const QString &key, bool value) {
    bool input = value;
    bool original = getBool(path, key, m_zooBackup);

    qDebug() << "Original value is: " << original;
    qDebug() << "Current input to compare: " << value;
    qDebug() << "Current value of m_dirty: " << m_dirty;
    if (input == original) {
        if (m_dirty > 0) {
            m_dirty--;
            emit dirtyChanged(m_dirty);
        }
        return;
    }

    m_zooini->SetValue(path.toStdString().c_str(), key.toStdString().c_str(),  QString::number(input).toStdString().c_str());
    m_dirty++;
    emit dirtyChanged(m_dirty);
    emit configUpdated(path, key, QString::number(value));
}

// TODO: Implement
void PZooConfig::updateUnlockEntity(const QString &key, const QString &value) {
    // m_unlockEntity.append(value);
    // m_configTable["user"][key] = value.toStdString();
    // emit unlockEntityUpdated(key, value);
}

// void PZooConfig::setZooConfigPath(const QString &path) {
//     if (m_zooConfigPath != path) {
//         m_zooConfigPath = path;
//         emit zooConfigPathChanged(m_zooConfigPath);
//     }
// }

// TODO: Add validation for unlockEntity and path=
void PZooConfig::saveConfig() {
    if (m_zooConfigPath.isEmpty()) {
        emit configError("Zoo.ini path is empty.");
        return;
    }

    if (m_dirty) {
        // get all sections
        CSimpleIniA::TNamesDepend sections;
        m_zooini->GetAllSections(sections);
        for (const auto &section : sections) {
            // remove empty keys from these sections
            if (QString(section.pItem) == "ai" || QString(section.pItem) == "scenario") {
                removeEmptyKeys(QString(section.pItem), "0");
            }
        }

        // save the settings to the file
        SI_ERROR rc = m_zooini->SaveFile(m_zooConfigPath.toStdString().c_str());
        if (rc < 0) {
            qDebug() << "Failed to save config file: " << m_zooConfigPath;
            emit configError("Failed to save config file.");
            return;
        }
        m_dirty = 0;

        // update the backup file
        copyIni(m_zooini, m_zooBackup);

        emit configSaved(m_zooConfigPath);
        emit dirtyChanged(m_dirty);
    } else {
        emit configError("No changes to save.");
    }
}

void PZooConfig::loadConfig() {
    QStringList validSections = {"debug", "mgr", "language", "lib", "resource", "user", "advanced", "Map", "UI", "scenario", "ai"};
    // load config to m emory
    m_zooini = std::make_unique<CSimpleIniA>();
    m_zooini->SetUnicode();
    SI_ERROR rc = m_zooini->LoadFile(m_zooConfigPath.toStdString().c_str());
    if (rc < 0) {
        qDebug() << "Failed to load config file: " << m_zooConfigPath;
        emit configError("Failed to load config file.");
        return;
    }


    if (!m_zooini) {
        qDebug() << "Failed to load config file: " << m_zooConfigPath;
        emit configError("Failed to load config file.");
        return;
    }

    // create backup of the config file
    m_zooBackup = std::make_unique<CSimpleIniA>();
    m_zooBackup->SetUnicode();
    // copy the config file to the backup
    copyIni(m_zooini, m_zooBackup);

    emit configLoaded(m_zooConfigPath);
}

// helper that removes empty keys from the settings
void PZooConfig::removeEmptyKeys(const QString &section, const QString &test) {
    // get all the keys in the section
    CSimpleIniA::TNamesDepend keys;
    m_zooini->GetAllKeys(section.toStdString().c_str(), keys);

    for (const auto &key : keys) {
        // check if the key is empty
        if (key.pItem == test.toStdString().c_str()) {
            // remove the key from the settings
            bool deleteSectionIfEmpty = true;
            m_zooini->Delete(section.toStdString().c_str(), key.pItem, deleteSectionIfEmpty);
        }
    }
}

void PZooConfig::revertChanges() {
    // revert the changes to the config file
    if (!m_zooBackup) {
        emit configError("No backup found to revert to.");
        return;
    }

    // copy the backup to the config file
    copyIni(m_zooBackup, m_zooini);

    m_dirty = 0;
    dirtyList.clear();
    emit configReverted();
    emit dirtyChanged(m_dirty);
}

bool PZooConfig::getBool(const QString &section, const QString &key, const std::unique_ptr<CSimpleIniA> &ini) const {
    const char* value;
    
    if (ini) {
        value = ini->GetValue(section.toStdString().c_str(), key.toStdString().c_str());
    } else {
        value = m_zooini->GetValue(section.toStdString().c_str(), key.toStdString().c_str());
    }

    // encode with utf8 (direct to std::string crashes because of encoding)
    QString valueStr = QString::fromUtf8(value);
    if (valueStr.isEmpty()) {
        return false;
    }

    // check if the value is 1 or true
    if (valueStr == "1" || valueStr == "true" || valueStr == "True") {
        return true;
    } else if (valueStr == "0" || valueStr == "false" || valueStr == "False") {
        return false;
    } else {
        return false;
    }
}

QString PZooConfig::getString(const QString &section, const QString &key, const std::unique_ptr<CSimpleIniA> &ini) const {
    const char* value;
    
    if (ini) {
        value = ini->GetValue(section.toStdString().c_str(), key.toStdString().c_str(), "");
    } else {
        value = m_zooini->GetValue(section.toStdString().c_str(), key.toStdString().c_str(), "");
    }
    if (value == nullptr) {
        return QString();
    }

    QString valueStr = QString::fromStdString(value);

    return valueStr;
}

void PZooConfig::copyIni(const std::unique_ptr<CSimpleIniA> &copyFrom, std::unique_ptr<CSimpleIniA> &copyTo) const {
    // clear the copyTo ini
    copyTo = std::make_unique<CSimpleIniA>();
    copyTo->SetUnicode();
    // copy the ini from the copyFrom ini

    // get sections
    CSimpleIniA::TNamesDepend sections;
    copyFrom->GetAllSections(sections);
    for (const auto &section : sections) {
        // get keys
        CSimpleIniA::TNamesDepend keys;
        copyFrom->GetAllKeys(section.pItem, keys);
        for (const auto &key : keys) {
            // get value
            const char* value = copyFrom->GetValue(section.pItem, key.pItem, "");
            if (value != nullptr) {
                copyTo->SetValue(section.pItem, key.pItem, value);
            }
        }
    }
}
