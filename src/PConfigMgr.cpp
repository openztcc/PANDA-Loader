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
// TODO: add validation for config files
std::vector<std::unique_ptr<PConfigMgr::IniData>> PConfigMgr::getAllConfigInZtd(const QString &ztdFilePath)
{
    QStringList validExtensions = { ".uca", ".ucb", ".ucs", ".ai", ".scn", ".cfg", ".ani" };
    std::vector<std::unique_ptr<PConfigMgr::IniData>> configFilesFound;

    // grab files for every valid extension
    for (const QString& ext : validExtensions) {
        QList<PZtdMgr::FileData> files = PZtdMgr::getFilesInZtd(ztdFilePath, ext);

        // store files in a list
        for (const auto& file : files) {
            auto iniData = byteArrayToIniData(file);
            configFilesFound.push_back(std::make_unique<PConfigMgr::IniData>(std::move(iniData)));
        }
    }

    return configFilesFound;
}


// Convert byte array to QSettings object
PConfigMgr::IniData PConfigMgr::byteArrayToIniData(const PZtdMgr::FileData& data)
{
    // write data to temp file
    QTemporaryFile tempFile;
    if (!tempFile.open()) {
        qWarning() << "Failed to open temporary file for INI data.";
        return {};
    }
    tempFile.write(data.data);
    tempFile.flush();

    // make qsettings from the temp file
    auto settings = std::make_unique<QSettings>(tempFile.fileName(), QSettings::IniFormat);

    PConfigMgr::IniData iniData;
    iniData.settings = std::move(settings);
    iniData.filename = data.filename;
    iniData.path = data.path;
    iniData.rawData = data.data;

    // close the temp file
    tempFile.close();

    // remove the temp file
    QFile::remove(tempFile.fileName());
    return iniData;
}

// Validates and returns the core config files in a ztd file
// TODO: add extension validation (only ucb, uca, ucs, ai)
std::vector<std::unique_ptr<PConfigMgr::IniData>> PConfigMgr::getCoreConfigInZtd(const QString &ztdFilePath)
{
    std::vector<std::unique_ptr<PConfigMgr::IniData>> configFiles = getAllConfigInZtd(ztdFilePath);
    return getCoreConfigInZtd(configFiles);
}

// Get the core config files from a ztd file
std::vector<std::unique_ptr<PConfigMgr::IniData>> PConfigMgr::getCoreConfigInZtd(std::vector<std::unique_ptr<PConfigMgr::IniData>> &configFiles)
{
    std::vector<std::unique_ptr<PConfigMgr::IniData>> coreConfigFiles;
    QStringList validRootFolders = { "animals", "scenery" };
    QStringList validSceneryFolders = { "buildings", "food", "other" };

    // Filter the config files to only include core configs
    for (auto& file : configFiles) {
        QStringList path = file->path.split("/");
        // check if the first folder is a valid root folder
        if (validRootFolders.contains(path[0])) {
            // check if the second folder is a valid scenery folder
            if (path.size() == 2 && path[0] == "animals") {
                coreConfigFiles.push_back(std::move(file));
            } else if (path.size() == 3 && path[0] == "scenery") {
                if (validSceneryFolders.contains(path[1])) {
                    coreConfigFiles.push_back(std::move(file));
                }
            }
        }
    }

    return coreConfigFiles;
}


// Get the icon animation paths from a ztd file
QStringList PConfigMgr::getIconAniPaths(std::vector<std::unique_ptr<PConfigMgr::IniData>> &configFiles)
{
    QStringList iconAniPaths;

    for (auto& file : configFiles) {
        if (!file->settings) continue;

        if (file->filename.endsWith(".uca")) {
            QStringList mIcon = PConfigMgr::extractDuplicateKeys(file->rawData, "m/Icon", "Icon");
            QStringList fIcon = PConfigMgr::extractDuplicateKeys(file->rawData, "f/Icon", "Icon");

            if (!mIcon.isEmpty()) {
                iconAniPaths.append(mIcon);
            }

            if (!fIcon.isEmpty()) {
                iconAniPaths.append(fIcon);
            }

        } else if (file->filename.endsWith(".ucb") || file->filename.endsWith(".ucs")) {
            QStringList icons = PConfigMgr::extractDuplicateKeys(file->rawData, "Icon", "Icon");
            if (!icons.isEmpty()) {
                iconAniPaths.append(icons);
            }
        }
    }

    return iconAniPaths;
}

// Overloaded function to get icon animation paths from a ztd file
QStringList PConfigMgr::getIconAniPaths(const QString &ztdFilePath)
{
    std::vector<std::unique_ptr<PConfigMgr::IniData>> configFiles = getAllConfigInZtd(ztdFilePath);

    return PConfigMgr::getIconAniPaths(configFiles);
}


// INI files by default are not allowed to have duplicate keys; blue fang
// config files tend to have duplicate keys in the same group
// This function extracts the duplicate keys from the INI data
// @iniData: the INI data as a QByteArray
// @group: the group to search for duplicate keys
// @key: the key to search for duplicate values
// @return: a QStringList of duplicate values
QStringList PConfigMgr::extractDuplicateKeys(const QByteArray& rawData, const QString& group, const QString& key)
{
    QStringList matches;
    QTextStream stream(rawData);
    bool inGroup = false;

    while (!stream.atEnd()) {
        QString line = stream.readLine().trimmed();

        if (line.startsWith("[") && line.endsWith("]")) {
            QString currentGroup = line.mid(1, line.length() - 2).trimmed();
            inGroup = (currentGroup.compare(group, Qt::CaseInsensitive) == 0);
            continue;
        }

        if (inGroup && line.startsWith(key, Qt::CaseInsensitive)) {
            QString value = line.section('=', 1).trimmed();
            if (!value.isEmpty()) {
                matches << value;
            }
        }
    }

    return matches;
}

std::vector<std::unique_ptr<PConfigMgr::IniData>> PConfigMgr::getIconAniConfigInZtd(const QString &ztdFilePath)
{
    std::vector<std::unique_ptr<PConfigMgr::IniData>> configFiles = getAllConfigInZtd(ztdFilePath);
    return getIconAniConfigInZtd(configFiles);
}

std::vector<std::unique_ptr<PConfigMgr::IniData>> PConfigMgr::getIconAniConfigInZtd(std::vector<std::unique_ptr<PConfigMgr::IniData>> &configFiles)
{
    QStringList extractedAniPaths = getIconAniPaths(configFiles);
    std::vector<std::unique_ptr<PConfigMgr::IniData>> iconAniConfigFiles;

    for (auto it = configFiles.begin(); it != configFiles.end(); ) {
        QString foundPath = (*it)->path.toLower();

        for (const QString& aniPath : extractedAniPaths) {
            QString extractedPath = (aniPath + ".ani").toLower();
            qDebug() << "Comparing " << foundPath << " with " << extractedPath;
            if (foundPath == extractedPath) {
                iconAniConfigFiles.push_back(std::move(*it));
                break; // break out of the inner loop
            }
        }

        ++it;
    }

    return iconAniConfigFiles;
}

// Get the paths to the icons from the icon animation config files
QStringList PConfigMgr::getIconPaths(std::vector<std::unique_ptr<PConfigMgr::IniData>> &aniFiles)
{
    QStringList iconPaths;

    for (auto& file : aniFiles) {
        if (!file->settings) continue;

        settings.beginGroup("animation");
        QStringList dirs;
        QStringList keys = settings.childKeys();
        for (const QString& key : keys) {
            if (key.startsWith("dir") || key == "animation") {
                dirs.append(settings.value(key).toString());
            }
        }
        settings.endGroup();
    }

    return iconPaths;
}

// Overloaded function to get icon paths from a ztd file
QStringList PConfigMgr::getIconPaths(const QString &ztdFilePath)
{
    std::vector<std::unique_ptr<PConfigMgr::IniData>> aniFiles = getIconAniConfigInZtd(ztdFilePath);
    return getIconPaths(aniFiles);
}

