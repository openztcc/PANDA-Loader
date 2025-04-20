#include "PConfigMgr.h"

const QString m_zooIniName = "zoo.ini";
const QString m_metaConfigName = "meta.toml";
const QString m_configDirPath = QDir::homePath() + "/.config/PandaLdr/"; // temporary
const QString m_metaConfigDirPath = m_configDirPath + "meta_configs/"; // temporary

PConfigMgr::PConfigMgr() {}

PConfigMgr::~PConfigMgr() {}

// Create a parser based on the file extension
std::unique_ptr<IConfigLoader> PConfigMgr::createParser(const QString &ext) const
{
    if (ext == "ini") {
        return std::make_unique<IIni>();
    } else if (ext == "toml") {
        return std::make_unique<IToml>();
    } else {
        return nullptr; // Unsupported file extension
    }
}

// Generic config parser for toml files in filesystem
bool PConfigMgr::loadConfig(const QString &filePath)
{
    // Get extension of the file
    QString ext = QFileInfo(filePath).suffix().toLower();
    m_config = createParser(ext);

    if (!m_config) {
        qDebug() << "Unsupported file extension: " << ext;
        return false;
    }

    // Load the config file
    if (!m_config->loadConfig(filePath)) {
        qDebug() << "Failed to load config file: " << filePath;
        return false;
    }

    return true;
}

// Save the config to disk
bool PConfigMgr::saveConfig(const QString &filePath)
{
    if (!m_config) {
        qDebug() << "Config parser not initialized";
        return false;
    }

    // Create the directory if it does not exist


    return true;
}

// Get a key value from config file
QVariant PConfigMgr::getValue(const QString &section, const QString &key)
{
    if (!m_config) {
        qDebug() << "Config parser not initialized";
        return QVariant();
    }

    return m_config->getValue(section, key);
}

// Set a key value in config file
void PConfigMgr::setValue(const QString &section, const QString &key, const QVariant &value)
{
    if (!m_config) {
        qDebug() << "Config parser not initialized";
        return;
    }

    m_config->setValue(section, key, value);
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

        file->settings->beginGroup("animation");

        QStringList dirs;
        QString animation;

        QStringList keys = file->settings->childKeys();
        for (const QString& key : keys) {
            if (key.startsWith("dir")) {
                dirs.append(file->settings->value(key).toString().trimmed());
            } else if (key == "animation") {
                animation = file->settings->value(key).toString().trimmed();
            }
        }

        file->settings->endGroup();

        if (!dirs.isEmpty() && !animation.isEmpty()) {
            QString fullPath = dirs.join("/") + "/" + animation;
            iconPaths.append(fullPath);
        }
    }

    return iconPaths;
}


// Overloaded function to get icon paths from a ztd file
QStringList PConfigMgr::getIconPaths(const QString &ztdFilePath)
{
    std::vector<std::unique_ptr<PConfigMgr::IniData>> aniFiles = getIconAniConfigInZtd(ztdFilePath);
    return getIconPaths(aniFiles);
}

