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
    toml::table config = PConfigMgr::getMetaConfig(filePath);
    if (config.empty()) {
        qDebug() << "Failed to load TOML file:" << filePath;
        return false;
    }

    // Load settings from the TOML file
    m_zooGamePath = PConfigMgr::getKeyValue("zooGamePath", config).c_str();
    m_useIsoMounting = PConfigMgr::getKeyValue("useIsoMounting", config) == "true";
    m_isoPath = PConfigMgr::getKeyValue("isoPath", config).c_str();

    return true;
}