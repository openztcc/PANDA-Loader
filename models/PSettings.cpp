#include "PSettings.h"

PSettings::PSettings(QObject* parent) : QObject(parent) {
    m_zooGamePath = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon";
    // Load settings from the TOML file
    if (!loadFromToml()) {
        qDebug() << "Failed to load settings from config.toml";
    }

    m_dirty = 0;
}

void PSettings::updateTable(const QString& key, const QString& value) {
    m_pandaConfig.insert_or_assign(key.toStdString(), value.toStdString());
    m_dirty = 1;
    emit dirtyChanged(m_dirty);
}

bool PSettings::loadFromToml() {
    QString configPath = m_configPath + "/config.toml";
    toml::table config = PConfigMgr::getConfig(configPath);
    if (config.empty()) {
        // generate default settings
        m_useIsoMounting = false;
        m_isoPath = "";
        // Save default settings to the TOML file
        toml::table defaultConfig;
        defaultConfig.insert_or_assign("zooGamePath", m_zooGamePath.toStdString());
        defaultConfig.insert_or_assign("isoPath", "");
        defaultConfig.insert_or_assign("useIsoMounting", false);

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

    // save the loaded config to the backup
    m_pandaBackupConfig = config;
    m_dirty = 0;
    m_pandaConfig = config;

    return true;
}

QString PSettings::getString(const QString& key) const {
    return QString::fromStdString(m_pandaConfig[key.toStdString()].as_string().value_or(""));
}

bool PSettings::getBool(const QString& key) const {
    return m_pandaConfig[key.toStdString()].as_boolean().value_or(false);
}
