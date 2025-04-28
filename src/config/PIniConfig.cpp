#include  "PIniConfig.h"

bool PIniConfig::loadConfig(const QString &filePath) {
    SI_Error rc = m_ini.LoadFile(filePath.toStdString().c_str());
    if (rc < 0) {
        qDebug() << "Failed to load INI file: " << filePath;
        return false;
    }
    return true;
}

bool PIniConfig::saveConfig(const QString &filePath) {
    SI_Error rc = m_ini.SaveFile(filePath.toStdString().c_str());
    if (rc < 0) {
        qDebug() << "Failed to save INI file: " << filePath;
        return false;
    }
    return true;
}

bool PIniConfig::clear() {
    m_ini.Reset();
    return true;
}

QVariant PIniConfig::getValue(const QString &section, const QString &key) const {
    const char *value = m_ini.GetValue(section.toStdString().c_str(), key.toStdString().c_str(), nullptr);
    if (value) {
        return extractVariant(QString(value));
    }
    return QVariant("");
}

void PIniConfig::setValue(const QString &key, const QVariant &value, const QString &section) {
    if (section == "") {
        qDebug() << "Error: No section provided for ini file with key = " << key << " and value " << value;
        return;
    }

    interpretVariant(m_ini, section.toStdString(), key.toStdString(), value);
}

bool PIniConfig::removeKey(const QString &key, const QString &section) {
    if (section.isEmpty()) {
        qDebug() << "Error: No section provided for ini file with key = " << key;
        return false;
    }
    bool deleteSectionIfEmpty = true;
    m_ini.Delete(section.toStdString().c_str(), key.toStdString().c_str(), deleteSectionIfEmpty);
    return true;
}

bool PIniConfig::removeSection(const QString &section) {
    return false;
}

bool PIniConfig::getAllSections() {
    return false;
}

QStringList PIniConfig::getAllKeys(const QString &section) {
    CSimpleIniA::TNamesDepend keys;
    m_ini.GetAllKeys(section.toStdString().c_str(), keys);
    QStringList keyList;
    for (const auto &key : keys) {
        keyList.append(QString(key.pItem));
    }
    return keyList;
}

// ---------------------------- Exist tests

bool PIniConfig::sectionExists(const QString &section) const {
    CSimpleIniA::TNamesDepend sections;
    m_ini.GetAllSections(sections);
    for (const auto &sec : sections) {
        if (section.compare(sec.pItem, Qt::CaseInsensitive) == 0) {
            return true;
        }
    }
    return false;
}

bool PIniConfig::keyExists(const QString &key, const QString &section) const {
    CSimpleIniA::TNamesDepend keys;

    // if section is empty, just check keys at the root level
    m_ini.GetAllKeys(section.isEmpty() ? nullptr : section.toStdString().c_str(), keys);

    // check if the key is empty
    for (const auto &k : keys) {
        if (key.compare(k.pItem, Qt::CaseInsensitive) == 0) {
            return true;
        }
    }
    return false;
}



bool PIniConfig::valueExists(const QString &value, const QString &key, const QString &section) const {
    // check if the key is empty
    const char* val = m_ini.GetValue(
        // ugly workaround for empty section but works
        section.isEmpty() ? nullptr : section.toStdString().c_str(),
        key.toStdString().c_str(),
        nullptr
    );

    // compare existance of the value
    return val && value.compare(val, Qt::CaseInsensitive) == 0;
}


// Removes all keys given a section and a value
void PIniConfig::removeKeysByValue(const QString &section, const QString &value) {
    // get all the keys in the section
    CSimpleIniA::TNamesDepend keys;
    m_ini.GetAllKeys(section.toStdString().c_str(), keys);

    for (const auto &key : keys) {
        // check if the key is empty
        if (key.pItem == value.toStdString().c_str()) {
            // remove the key from the settings
            bool deleteSectionIfEmpty = true;
            m_ini.Delete(section.toStdString().c_str(), key.pItem, deleteSectionIfEmpty);
        }
    }
}


// --------------- Operator overloads

PIniConfig& PIniConfig::operator=(const PIniConfig& other) {
    if (this == &other) {
        return *this; // self-assignment check
    }

    // // copy the ini from the other ini
    m_ini.Reset();
    m_ini.SetUnicode(other.m_ini.IsUnicode());

    // copy all sections and keys
    CSimpleIniA::TNamesDepend sections;
    other.m_ini.GetAllSections(sections);
    for (const auto& section : sections) {
        CSimpleIniA::TNamesDepend keys;
        other.m_ini.GetAllKeys(section.pItem, keys);
        for (const auto& key : keys) {
            const char* value = other.m_ini.GetValue(section.pItem, key.pItem, nullptr);
            if (value) {
                m_ini.SetValue(section.pItem, key.pItem, value);
            }
        }
    }
    return *this;
}


// --------------- Private functions

// ini files in ZT only have bools, ints, floats, and strings
void PIniConfig::interpretVariant(CSimpleIniA& config, const std::string& section, const std::string& key, const QVariant& value) {
    // validate
    if (key.empty()) {
        qDebug() << "Error: No key provided for ini file with value " << value;
        return;
    }

    if (value.isNull()) {
        qDebug() << "Error: No value provided for ini file with key = " << key;
        config.SetValue(section.c_str(), key.c_str(), "");
        return;
    }

    if (value.isValid() == false) {
        qDebug() << "Error: Invalid value provided for ini file with key = " << key;
        return;
    }

    config.SetValue(section.c_str(), key.c_str(), value.toString().toStdString().c_str());
}

// Extracts the value from the ini file and returns it as a QVariant
QVariant PIniConfig::extractVariant(const QString& query) const {
// try reading as int or double
    bool ok = false;
    int intVal = query.toInt(&ok);
    if (ok) {
        return intVal;
    }

    double dblVal = query.toFloat(&ok);
    if (ok) {
        return dblVal;
    }

    // at this point assume it is a string,
    // but check if it is a boolean string
    if (query == "true") {
        return 1;
    } else if (query == "false") {
        return 0;
    }

    return QVariant(query); // interpret as string
}


// -------------------- TODO

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

// // TODO: Implement
// void PZooConfig::updateUnlockEntity(const QString &key, const QString &value) {
//     // m_unlockEntity.append(value);
//     // m_configTable["user"][key] = value.toStdString();
//     // emit unlockEntityUpdated(key, value);
// }

// // void PZooConfig::setZooConfigPath(const QString &path) {
// //     if (m_zooConfigPath != path) {
// //         m_zooConfigPath = path;
// //         emit zooConfigPathChanged(m_zooConfigPath);
// //     }
// // }

// // helper that removes empty keys from the settings
// void PZooConfig::removeEmptyKeys(const QString &section, const QString &test) {
//     // get all the keys in the section
//     CSimpleIniA::TNamesDepend keys;
//     m_zooini->GetAllKeys(section.toStdString().c_str(), keys);

//     for (const auto &key : keys) {
//         // check if the key is empty
//         if (key.pItem == test.toStdString().c_str()) {
//             // remove the key from the settings
//             bool deleteSectionIfEmpty = true;
//             m_zooini->Delete(section.toStdString().c_str(), key.pItem, deleteSectionIfEmpty);
//         }
//     }
// }