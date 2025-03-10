#include "PConfigMgr.h"

const QString m_zooIniName = "zoo.ini";
const QString m_metaConfigName = "meta.toml";
const QString m_configDirPath = QDir::homePath() + "/.config/PandaLdr/"; // temporary
const QString m_metaConfigDirPath = m_configDirPath + "meta_configs/"; // temporary

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

// Get a key value from a toml table
QString PConfigMgr::getKeyValue(const QString &key, const toml::table &config)
{
    // Find value in table
    if (auto it = config.find(key.toStdString()); it != config.end()) {
        if (auto strVal = it->second.as_string()) {
            return QString::fromStdString(strVal->get());
        }
    }

    return QString("");
}

// Updates the meta configuration in a ztd file
// TODO: Test if this updates the config correctly
// - Possible issue with old config not being removed
// - If successful, remove addMetaConfig. Needs condition to check if config exists
bool PConfigMgr::updateMetaConfig(const QString &ztdFilePath, const toml::table &config)
{
    // Check if the ztd file exists
    if (!PZtdMgr::isZtdFile(ztdFilePath)) {
        qDebug() << "Ztd file does not exist or is not valid";
        return false; // Ztd file does not exist or is not valid
    }

    // // Create a temporary file to store the new ztd with the updated config
    // QString tempZtdPath = ztdFilePath + ".tmp";

    // // Copy the original ztd to the temporary file
    // if (!PZtdMgr::copyZtdFile(ztdFilePath, tempZtdPath)) {
    //     qDebug() << "Failed to copy ztd file";
    //     return false; // Failed to copy the ztd file
    // }

    // Remove the old meta configuration in ztd
    if (!PConfigMgr::removeMetaConfig(ztdFilePath)) {
        qDebug() << "Failed to remove old meta config from ztd";
        return false; // Failed to remove the old meta config
    }

    // Update the meta configuration in the temporary ztd
    std::ostringstream oss;
    oss << config;
    QByteArray tomlData = QByteArray::fromStdString(oss.str());
    if (!PZtdMgr::addFileToZtd(ztdFilePath, m_metaConfigName)) {
        qDebug() << "Failed to add new meta config to ztd";
        oss.clear();
        return false; // Failed to update the meta config
    }

    // // Replace the original ztd with the temporary one
    // if (!PZtdMgr::moveZtdFile(tempZtdPath, ztdFilePath)) {
    //     qDebug() << "Failed to replace old ztd file";
    //     QFile::remove(tempZtdPath); // Clean up temporary file
    //     return false; // Failed to replace the original ztd
    // }

    return true;
}

// Removes a meta configuration from a ztd file (if it exists)
// TODO: Test if this removes the config correctly
bool PConfigMgr::removeMetaConfig(const QString &ztdFilePath)
{
    // Check if valid zip file
    QuaZip zip(ztdFilePath);
    if (!zip.open(QuaZip::mdUnzip)) {
        qDebug() << "Failed to open ZTD file for extraction: " << zip.getZipError();
        return false;
    }
    zip.close();

    // Create a temporary file to store the new ztd without the meta config
    QString tempZtdPath = ztdFilePath;

    // // Copy the original ztd to the temporary file
    // if (!PZtdMgr::copyZtdFile(ztdFilePath, tempZtdPath)) {
    //     qDebug() << "Failed to copy ztd file";
    //     return false; // Failed to copy the ztd file
    // }

    // Remove the meta configuration from the temporary ztd
    if (!PZtdMgr::removeFileFromZtd(tempZtdPath, m_metaConfigName)) {
        QFile::remove(tempZtdPath); // Clean up temporary file
        qDebug() << "Failed to remove meta config from ztd";
        return false; // Failed to remove the meta config
    }

    // // Replace the original ztd with the temporary one
    // if (!PZtdMgr::moveZtdFile(tempZtdPath, ztdFilePath)) {
    //     qDebug() << "Failed to replace old ztd file";
    //     QFile::remove(tempZtdPath); // Clean up temporary file
    //     return false; // Failed to replace the original ztd
    // }

    return true;
}

// Get the zoo.ini configuration from Zoo Tycoon installation
toml::table PConfigMgr::getZooIniConfig(const QString &iniPath)
{
    // Check if the ini file exists
    if (!QFile::exists(iniPath)) {
        return toml::table(); // Return empty table if the ini file does not exist
    }

    // Read the ini file
    QSettings settings(iniPath, QSettings::IniFormat);
    toml::table config;

    // Iterate through all keys in the ini file and add them to the toml table
    for (const QString &key : settings.allKeys()) {
        config.insert_or_assign(key.toStdString(), toml::value(settings.value(key).toString().toStdString()));
    }

    return config;
}

// Updates the zoo.ini configuration
bool PConfigMgr::updateZooIniConfig(const QString &iniPath, const toml::table &config)
{
    // Check if the ini file exists
    if (!QFile::exists(iniPath)) {
        return false;
    }

    // Write the new configuration to the ini file
    QSettings settings(iniPath, QSettings::IniFormat);

    for (const auto &[key, value] : config) {
        const auto* str_value = value.as_string();
        std::string str = str_value ? str_value->get() : "";

        settings.setValue(QString::fromStdString(std::string(key)), QString::fromStdString(str));
    }

    return true;
}

// Removes the zoo.ini configuration
bool PConfigMgr::removeZooIniConfig(const QString &iniPath)
{
    // Check if the ini file exists
    if (!QFile::exists(iniPath)) {
        return false; // Return false if the ini file does not exist
    }

    // Remove the ini file
    return QFile::remove(iniPath);
}

