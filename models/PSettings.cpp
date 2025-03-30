#include "PSettings.h"

void PSettings::setZooGamePath(const QString& path) {
    if (m_zooGamePath != path) {
        m_zooGamePath = path;
        emit zooGamePathChanged();
    }
}

void PSettings::setUseIsoMounting(bool use) {
    if (m_useIsoMounting != use) {
        m_useIsoMounting = use;
        emit useIsoMountingChanged();
    }
}

void PSettings::setIsoPath(const QString& path) {
    if (m_isoPath != path) {
        m_isoPath = path;
        emit isoPathChanged();
    }
}

PSettings::PSettings(QObject* parent) : QObject(parent) {
    // Load settings from the TOML file
    loadFromToml(m_configPath);
}

bool PSettings::loadFromToml(const QString& filePath) {
    toml::table config = PConfigMgr::getConfig(filePath);
    if (config.empty()) {
        // generate default settings
        m_zooGamePath = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\zoo.exe";
        m_useIsoMounting = false;
        m_isoPath = "";
        // Save default settings to the TOML file
        toml::table defaultConfig;
        defaultConfig["zooGamePath"] = m_zooGamePath.toStdString();
        defaultConfig["useIsoMounting"] = m_useIsoMounting;
        defaultConfig["isoPath"] = m_isoPath.toStdString();
        // Save the default config to the file
        
    }

    // Load settings from the TOML file
    m_zooGamePath = PConfigMgr::getKeyValue("zooGamePath", config).c_str();
    m_useIsoMounting = PConfigMgr::getKeyValue("useIsoMounting", config) == "true";
    m_isoPath = PConfigMgr::getKeyValue("isoPath", config).c_str();

    return true;
}