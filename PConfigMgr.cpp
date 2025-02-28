#include "PConfigMgr.h"

PConfigMgr::PConfigMgr() {}

PConfigMgr::~PConfigMgr() {}

// Get the meta configuration from a ztd file
toml::table PConfigMgr::getMetaConfig(const QString &ztdFilePath)
{
    // Get toml file from ztd
    toml::table config;
    QByteArray fileData;

    // Check if the ztd file exists and parse
    if (PZtdMgr::openFileInZtd(ztdFilePath, m_metaConfigName, fileData)) {
        // Parse the toml data
        config = toml::parse(fileData.constData());
    }

    if (config.empty()) {
        // If the config is empty, return an empty toml table
        return toml::table();
    }

    return config;
}