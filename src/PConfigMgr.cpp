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

    // does file exist
    if (PZtdMgr::openFileInZtd(ztdFilePath, m_metaConfigName, fileData)) {
        config = toml::parse(fileData.constData());
    }

    if (config.empty()) {
        return toml::table();
    }

    return config;
}

// Generic config parser for toml files in filesystem
toml::table PConfigMgr::getConfig(const QString &filePath)
{
    // Check if the config file exists
    if (!QFile::exists(filePath)) {
        return toml::table();
    }

    // Read config file
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return toml::table();
    }

    QByteArray fileData = file.readAll();
    file.close();

    // Parse the toml data
    toml::table config = toml::parse(fileData.constData());

    return config;
}

// Save the config to a toml file in filesystem
bool PConfigMgr::saveConfig(const QString &filePath, const toml::table &config)
{
    // Create the directory if it does not exist
    QDir dir(QFileInfo(filePath).absolutePath());
    if (!dir.exists()) {
        dir.mkpath(dir.absolutePath());
    }

    // Write the new configuration to the toml file
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false; 
    }

    std::stringstream ss;
    ss << config;
    QString tomlStr = QString::fromStdString(ss.str());
    file.write(tomlStr.toUtf8());
    file.close();

    return true;
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

// Get a key value from a toml table as a boolean
bool PConfigMgr::getBoolValue(const QString &key, const toml::table &config)
{
    // Find value in table
    if (auto it = config.find(key.toStdString()); it != config.end()) {
        if (auto boolVal = it->second.as_boolean()) {
            return boolVal->get();
        }
    }

    return false;
}

// Get a key value from a toml table as a list (ie tags and authors)
QVector<QString> PConfigMgr::getKeyValueAsList(const QString &key, const toml::table &config)
{
    QVector<QString> result;

    // Find value in table
    if (auto it = config.find(key.toStdString()); it != config.end()) {
        if (auto arrVal = it->second.as_array()) {
            for (const auto &item : *arrVal) {
                if (auto strVal = item.as_string()) {
                    result.append(QString::fromStdString(strVal->get()));
                }
            }
        }
    }

    return result;
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

    QString metaConfigPath = QDir::tempPath() + "/" + m_metaConfigName;
    QString tempZtdPath = QDir::tempPath() + "/" + QUuid::createUuid().toString(QUuid::WithoutBraces) + ".ztd";

    // Copy the original ztd to the temporary file
    if (!PZtdMgr::copyZtdFile(ztdFilePath, tempZtdPath)) {
        qDebug() << "Failed to copy ztd file";
        qDebug() << "From " << ztdFilePath << " to " << tempZtdPath;
        return false; // Failed to copy the ztd file
    }

    // if meta.toml already exists, remove it
    if (PZtdMgr::fileExistsInZtd(tempZtdPath, m_metaConfigName)) {
        if (!PZtdMgr::removeFileFromZtd(tempZtdPath, m_metaConfigName)) {
            qDebug() << "Failed to remove old meta config from ztd";
            return false; // Failed to remove the old meta config
        }
    }

    // Create a temporary file to store the new meta config
    QFile tempMeta(metaConfigPath);
    if (!tempMeta.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to create temporary meta config file";
        tempMeta.remove(); // Cleanup temp file
        return false; // Failed to create temporary meta config file
    }
    std::stringstream ss;
    
    ss << config;
    QString tomlStr = QString::fromStdString(ss.str());
    tempMeta.write(tomlStr.toUtf8());
    tempMeta.close();
    
    // Add the TOML file to the ZTD archive
    if (!PZtdMgr::addFileToZtd(tempZtdPath, metaConfigPath)) {
        qDebug() << "Failed to add new meta config to ztd";
        tempMeta.remove(); // Cleanup temp file
        return false;
    }

    // Remove old ztd file
    if (!QFile::remove(ztdFilePath)) {
        qDebug() << "Failed to remove old ztd file";
        tempMeta.remove(); // Cleanup temp file
        return false;
    }

    // Replace the original ztd with the temporary one
    if (!QFile::rename(tempZtdPath, ztdFilePath)) {
        qDebug() << "Failed to replace old ztd file";
        tempMeta.remove(); // Cleanup temp file
        return false; // Failed to replace the original ztd
    }

    tempMeta.remove(); // Cleanup temp file
    QFile::remove(metaConfigPath); // Cleanup temp file

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

// Reads a Panda config file and returns a toml table
bool PConfigMgr::readPandaConfig(const QString &filePath, toml::table &config)
{
    // Check if config file exists
    if (!QFile::exists(filePath)) {
        return false;
    }

    // Read config file
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QByteArray fileData = file.readAll();
    file.close();

    // Parse the toml data
    config = toml::parse(fileData.constData());

    return true;
}

// Get config from ztd file
QList<std::unique_ptr<QSettings>> PConfigMgr::getConfigInZtd(const QString &ztdFilePath, const QString &ext, const QString &entityType)
{
    QStringList validFolders = { "scenery", "animals" };
    QStringList validExtensions = { ".uca", ".ucb", ".ucs", ".ai", ".scn", ".cfg", ".ani" };
    QList<std::unique_ptr<QSettings>> configFilesFound;

    QStringList folderFilter = entityType.isEmpty() ? validFolders : QStringList{ entityType };
    QStringList extensionFilter = ext.isEmpty() ? validExtensions : QStringList{ ext };

    // Validate input
    if (!ext.isEmpty() && !validExtensions.contains(ext, Qt::CaseInsensitive)) {
        qDebug() << "Invalid file extension:" << ext;
        return configFilesFound;
    }
    if (!entityType.isEmpty() && !validFolders.contains(entityType, Qt::CaseInsensitive)) {
        qDebug() << "Invalid folder name:" << entityType;
        return configFilesFound;
    }

    QuaZip zip(ztdFilePath);
    if (!zip.open(QuaZip::mdUnzip)) {
        qDebug() << "Failed to open ZTD file:" << zip.getZipError();
        return configFilesFound;
    }

    QStringList fileList = zip.getFileNameList();
    QuaZipFile zipFile(&zip);

    for (const QString &fileName : fileList) {
        // Filter by folder
        bool folderMatch = false;
        for (const QString &folder : folderFilter) {
            if (fileName.startsWith(folder + "/", Qt::CaseInsensitive)) {
                folderMatch = true;
                break;
            }
        }
        if (!folderMatch) continue;

        // Filter by extension
        bool extensionMatch = false;
        for (const QString &fileExt : extensionFilter) {
            if (fileName.endsWith(fileExt, Qt::CaseInsensitive)) {
                extensionMatch = true;
                break;
            }
        }
        if (!extensionMatch) continue;

        // Open the file inside the zip
        if (!zip.setCurrentFile(fileName)) {
            qDebug() << "Failed to select file in ZTD:" << fileName;
            continue;
        }
        if (!zipFile.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to open file in ZTD:" << fileName;
            continue;
        }

        QByteArray fileData = zipFile.readAll();
        zipFile.close();

        // in memory buffer for QSettings
        auto buffer = std::make_unique<QBuffer>();
        buffer->setData(fileData);
        buffer->open(QIODevice::ReadOnly);

        // auto settings = std::make_unique<QSettings>(buffer.get(), QSettings::IniFormat);
        // transf ownership to QSettings
        buffer.release(); 

        // configFilesFound.append(std::move(settings));
    }

    zip.close();
    return configFilesFound;
}

// Get known config in ztd file with a specific path
std::unique_ptr<QSettings> PConfigMgr::getKnownConfigInZtd(const QString &ztdFilePath, const QString &path)
{
    QuaZip zip(ztdFilePath);
    if (!zip.open(QuaZip::mdUnzip)) {
        qDebug() << "Failed to open ZTD file:" << zip.getZipError();
        return nullptr;
    }

    QuaZipFile zipFile(&zip);
    if (!zip.setCurrentFile(path)) {
        qDebug() << "Failed to select file in ZTD:" << path;
        return nullptr;
    }
    if (!zipFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file in ZTD:" << path;
        return nullptr;
    }

    QByteArray fileData = zipFile.readAll();
    zipFile.close();

    // in memory buffer for QSettings
    auto buffer = std::make_unique<QBuffer>();
    buffer->setData(fileData);
    buffer->open(QIODevice::ReadOnly);

    // auto settings = std::make_unique<QSettings>(buffer.get(), QSettings::IniFormat);
    // transf ownership to QSettings
    buffer.release(); 

    return nullptr; // TODO: Return the settings object
}
