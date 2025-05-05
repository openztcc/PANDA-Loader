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

PConfigMgr::PConfigMgr(QObject *parent, const QSharedPointer<PFileData> &fileData) : QObject(parent), m_dirty(0), m_config(nullptr), m_configBackup(nullptr), m_dirty_laundry(nullptr)
{
    if (fileData) {
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
    if (ext == "ini" || ext == "uca" || ext == "ucb" || ext == "ucs" || ext == "ai" || ext == "ani" || ext == "cfg") {
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
bool PConfigMgr::loadConfig(const QSharedPointer<PFileData> &fileData)
{
    if (!fileData) {
        qDebug() << "No data provided for config file";
        return false;
    }

    // Create a temporary file to load the config from
    QString tempFilePath = QDir::tempPath() + "/" + fileData->filename + "." + fileData->ext;
    QFile tempFile(tempFilePath);
    if (!tempFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to create temporary file: " << tempFilePath;
        return false;
    } else {
        qDebug() << "Temporary file created: " << tempFilePath;
    }
    tempFile.write(fileData->data);
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
        qDebug() << "After trying to saveConfig, config parser not initialized";
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
        qDebug() << "After trying to clear(), config parser not initialized";
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
        qDebug() << "Tried to get key " << key << " from section [" << section << "] but config parser not initialized";
        return QVariant();
    }

    return m_config->getValue(section, key);
}

// Get a key value from config file with support for multiple keys
QVariant PConfigMgr::getValue(const QString &section, const QString &key, bool getMultiKeys) const
{
    if (!getMultiKeys) {
        return m_config->getValue(section, key);
    }

    if (!m_config) {
        qDebug() << "Tried to get multiple keys " << key << " from section [" << section << "] but config parser not initialized";
        return QVariant();
    }

    return m_config->getValue(section, key, getMultiKeys);
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
        qDebug() << "Tried to set key " << key << " in section [" << section << "] but config parser not initialized";
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

// Returns all keys in the config file within a section
QStringList PConfigMgr::getAllKeys(const QString &section) const
{
    if (!m_config) {
        qDebug() << "Tried to get all keys from section [" << section << "] but config parser not initialized";
        return QStringList();
    }

    // Get all keys in the config file within a section
    QStringList keys = m_config->getAllKeys(section);
    return keys;
}
