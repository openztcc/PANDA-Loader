#include "PConfigMgr.h"

const QString m_zooIniName = "zoo.ini";
const QString m_metaConfigName = "meta.toml";
const QString m_configDirPath = QDir::homePath() + "/.config/PandaLdr/"; // temporary
const QString m_metaConfigDirPath = m_configDirPath + "meta_configs/"; // temporary

PConfigMgr::PConfigMgr(QObject *parent, const QString &filepath) : QObject(parent), m_dirty(0), m_config(nullptr), m_configBackup(nullptr), m_dirty_laundry(nullptr)
{
    m_dirty = 0;
    
    if (!filepath.isEmpty()) {
        loadConfig(filepath);
    } else {
        qDebug() << "No file path provided for PConfigMgr: " << filepath;
    }
}

PConfigMgr::PConfigMgr(QObject *parent, const PFileData &fileData) : QObject(parent), m_dirty(0), m_config(nullptr), m_configBackup(nullptr), m_dirty_laundry(nullptr)
{
    if (!fileData.data.isEmpty()) {
        loadConfig(fileData);
    } else {
        qDebug() << "No data provided for config file";
    }
}

PConfigMgr::~PConfigMgr() {}

// Create a parser based on the file extension
std::unique_ptr<IConfigLoader> PConfigMgr::createParser(const QString &path) const
{
    // Get the file extension
    QString ext = QFileInfo(path).suffix().toLower();
    if (ext == "ini") {
        return std::make_unique<PIniConfig>();
    } else if (ext == "toml") {
        return std::make_unique<PTomlConfig>();
    } else {
        return nullptr; // Unsupported file extension
    }
}

// Generic config parser for toml files in filesystem
bool PConfigMgr::loadConfig(const QString &filePath)
{
    // Get extension of the file
    QString ext = QFileInfo(filePath).suffix().toLower();
    m_config = createParser(filePath);

    if (!m_config) {
        qDebug() << "Unsupported file extension: " << ext;
        return false;
    }

    // Load the config file
    if (!m_config->loadConfig(filePath)) {
        qDebug() << "Failed to load config file: " << filePath;
        return false;
    }

    m_dirty = 0; // Reset dirty flag
    if (!m_dirty_laundry) {
        m_dirty_laundry = createParser(filePath); // Create a new dirty laundry list
    } else {
        m_dirty_laundry->clear(); // Clear the dirty laundry list
    }
    m_configPath = filePath; // Store the config path
    m_configBackup = m_config->clone(); // Create a backup of the config

    QString filename = QFileInfo(filePath).fileName();
    
    // emit dirtyChanged signal
    emit dirtyChanged(m_dirty);
    qDebug() << filename << " loaded successfully.";
    qDebug() << "Config path: " << m_configPath;

    return true;
}

// Load config from a PFileData object
// TODO: Eventually rework this to use the PFileData object directly instead of creating a temporary file
bool PConfigMgr::loadConfig(const PFileData &fileData)
{
    if (fileData.data.isEmpty()) {
        qDebug() << "No data provided for config file";
        return false;
    }

    // Create a temporary file to load the config from
    QString tempFilePath = QDir::tempPath() + "/" + QUuid::createUuid().toString(QUuid::WithoutBraces) + ".tmp.ini";
    QFile tempFile(tempFilePath);
    if (!tempFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to create temporary file: " << tempFilePath;
        return false;
    }
    tempFile.write(fileData.data);
    tempFile.close();

    // Load the config from the temporary file
    bool result = loadConfig(tempFilePath);
    QFile::remove(tempFilePath); // Remove the temporary file

    return result;
}

// Save the config to disk
bool PConfigMgr::saveConfig(const QString &filePath)
{
    if (!m_config) {
        qDebug() << "Config parser not initialized";
        return false;
    }

    m_config->saveConfig(filePath);

    m_dirty = 0; // Reset dirty flag
    m_configBackup = m_config->clone(); // Create a backup of the config
    m_dirty_laundry->clear(); // Clear the dirty laundry list
    emit dirtyChanged(m_dirty);

    return true;
}

// Save the config to the default path
bool PConfigMgr::saveConfig()
{
    return saveConfig(m_configPath);
}

// Revert changes to the config file
bool PConfigMgr::revertChanges()
{
    if (!m_config) {
        qDebug() << "Unable to revert changes: config parser not initialized";
        return false;
    }

    // Revert the changes to the config file
    m_config = m_configBackup->clone(); // Restore the backup
    m_dirty = 0; // Reset dirty flag
    m_dirty_laundry->clear(); // Clear the dirty laundry list
    emit dirtyChanged(m_dirty);

    return true;
}

// Clear the config
bool PConfigMgr::clear()
{
    if (!m_config) {
        qDebug() << "Config parser not initialized";
        return false;
    }

    m_config->clear();
    m_configBackup->clear();
    m_dirty = 0; // Reset dirty flag
    m_dirty_laundry->clear(); // Clear the dirty laundry list
    emit dirtyChanged(m_dirty);

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
void PConfigMgr::setValue(const QString &key, const QVariant &value, const QString &section)
{
    // get path extension
    // QString ext = QFileInfo(m_configPath).suffix().toLower();
    QVariant input = m_config->extractVariant(value.toString());

    QVariant originalValue = m_configBackup->getValue(section, key);
    // interpret original as well  because we now know the type
    if (input.typeId() == QMetaType::Int && originalValue == "") {
        originalValue = 0;
    }

    qDebug() << "Original value: " << originalValue << " vs " << input;
    if (originalValue.toString() == input.toString()) {
        if (m_dirty > 0) {
            if (m_dirty_laundry->valueExists(originalValue.toString(), key, section)) {
                m_dirty_laundry->removeKey(key, section);
                m_dirty--;
                emit dirtyChanged(m_dirty);
                qDebug() << "Dirty laundry removed one item. Current count: " << m_dirty;
            }
        }
        return;
    }

    if (!m_config) {
        qDebug() << "Config parser not initialized";
        return;
    }

    if (!m_dirty_laundry->valueExists(originalValue.toString(), key, section)) {
        m_dirty++;
        m_dirty_laundry->setValue(key, originalValue, section);
        emit dirtyChanged(m_dirty);
        qDebug() << "Dirty laundry updated. Current count: " << m_dirty;
    }
    m_config->setValue(key, input, section);
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

