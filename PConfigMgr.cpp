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

// Updates the meta configuration in a ztd file
// TODO: Test if this updates the config correctly
// - Possible issue with old config not being removed
// - If successful, remove addMetaConfig. Needs condition to check if config exists
bool PConfigMgr::updateMetaConfig(const QString &ztdFilePath, const toml::table &config)
{
    // Check if the ztd file exists
    if (!PZtdMgr::isZtdFile(ztdFilePath)) {
        return false; // Ztd file does not exist or is not valid
    }

    // Create a temporary file to store the new ztd with the updated config
    QString tempZtdPath = ztdFilePath + ".tmp";

    // Copy the original ztd to the temporary file
    if (!PZtdMgr::copyZtdFile(ztdFilePath, tempZtdPath)) {
        return false; // Failed to copy the ztd file
    }

    // Update the meta configuration in the temporary ztd
    std::ostringstream oss;
    oss << config;
    QByteArray tomlData = QByteArray::fromStdString(oss.str());
    if (!PZtdMgr::addFileToZtd(tempZtdPath, m_metaConfigName)) {
        QFile::remove(tempZtdPath); // Clean up temporary file
        return false; // Failed to update the meta config
    }

    // Replace the original ztd with the temporary one
    if (!PZtdMgr::moveZtdFile(tempZtdPath, ztdFilePath)) {
        QFile::remove(tempZtdPath); // Clean up temporary file
        return false; // Failed to replace the original ztd
    }

    return true;
}

// Removes a meta configuration from a ztd file (if it exists)
// TODO: Test if this removes the config correctly
bool PConfigMgr::removeMetaConfig(const QString &ztdFilePath)
{
    // Check if the ztd file exists
    if (!PZtdMgr::isZtdFile(ztdFilePath)) {
        return false; // Ztd file does not exist or is not valid
    }

    // Create a temporary file to store the new ztd without the meta config
    QString tempZtdPath = ztdFilePath + ".tmp";

    // Copy the original ztd to the temporary file
    if (!PZtdMgr::copyZtdFile(ztdFilePath, tempZtdPath)) {
        return false; // Failed to copy the ztd file
    }

    // Remove the meta configuration from the temporary ztd
    if (!PZtdMgr::removeFileFromZtd(tempZtdPath, m_metaConfigName)) {
        QFile::remove(tempZtdPath); // Clean up temporary file
        return false; // Failed to remove the meta config
    }

    // Replace the original ztd with the temporary one
    if (!PZtdMgr::moveZtdFile(tempZtdPath, ztdFilePath)) {
        QFile::remove(tempZtdPath); // Clean up temporary file
        return false; // Failed to replace the original ztd
    }

    return true;
}

// Adds a meta configuration to a ztd file
// TODO: Test if this adds the config correctly
bool PConfigMgr::addMetaConfig(const QString &ztdFilePath, const toml::table &config)
{
    // Check if the ztd file exists
    if (!PZtdMgr::isZtdFile(ztdFilePath)) {
        return false; // Ztd file does not exist or is not valid
    }

    // Create a temporary file to store the new ztd with the added config
    QString tempZtdPath = ztdFilePath + ".tmp";

    // Copy the original ztd to the temporary file
    if (!PZtdMgr::copyZtdFile(ztdFilePath, tempZtdPath)) {
        return false; // Failed to copy the ztd file
    }

    // Add the meta configuration to the temporary ztd
    std::ostringstream oss;
    oss << config;
    QByteArray tomlData = QByteArray::fromStdString(oss.str());
    if (!PZtdMgr::addFileToZtd(tempZtdPath, m_metaConfigName)) {
        QFile::remove(tempZtdPath); // Clean up temporary file
        return false; // Failed to add the meta config
    }

    // Replace the original ztd with the temporary one
    if (!PZtdMgr::moveZtdFile(tempZtdPath, ztdFilePath)) {
        QFile::remove(tempZtdPath); // Clean up temporary file
        return false; // Failed to replace the original ztd
    }

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

