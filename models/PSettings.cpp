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
    loadFromToml();
}

bool PSettings::loadFromToml() {
    QString configPath = m_configPath + "/config.toml";
    toml::table config = PConfigMgr::getConfig(configPath);
    if (config.empty()) {
        // generate default settings
        m_zooGamePath = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon";
        m_useIsoMounting = false;
        m_isoPath = "";
        // Save default settings to the TOML file
        toml::table defaultConfig;
        defaultConfig.insert_or_assign("zooGamePath", m_zooGamePath.toStdString());
        defaultConfig.insert_or_assign("isoPath", m_isoPath.toStdString());
        defaultConfig.insert_or_assign("useIsoMounting", m_useIsoMounting);

        // Save the default config to the file
        if (!PConfigMgr::saveConfig(configPath, defaultConfig)) {
            qDebug() << "Failed to save default settings to config.toml";
            return false;
        }

        qDebug() << "Default settings saved to config.toml";

        // replace the config with the default one
        config = defaultConfig;
    }

    // Load settings from the TOML file
    m_zooGamePath = PConfigMgr::getKeyValue("zooGamePath", config);
    m_useIsoMounting = PConfigMgr::getBoolValue("useIsoMounting", config);
    m_isoPath = PConfigMgr::getKeyValue("isoPath", config);

    return true;
}
