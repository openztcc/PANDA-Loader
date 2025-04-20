#include "PState.h"
#include <qdir.h>

PState::PState(QObject *parent) : QObject(parent) {
    m_path = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\";
    m_resource_path = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\dlupdate\\";
    m_mods = QVector<PModItem>();
    m_pandacfg = new PConfigMgr(this, );
    // Load settings from the TOML file
    if (!m_pandacfg->loadFromToml()) {
        qDebug() << "Failed to load settings from config.toml";
    } else {
        m_path = m_settings->zooGamePath();
        m_resource_path = m_settings->zooGamePath() + "\\dlupdate\\";
        qDebug() << "Loaded settings from config.toml.";
    }

    // Load the Zoo Tycoon config file
    m_zooConfig = new PZooConfig(this, m_path + "\\zoo.ini");
    if (!m_zooConfig) {
        qDebug() << "Failed to load Zoo Tycoon config file.";
    } else {
        m_zooConfig->loadConfig();
        qDebug() << "Loaded Zoo Tycoon config file.";
    }
}

int PState::launchZT() {
    // QMutexLocker locker(&mutex);
    QString _path = this->m_path + "\\zoo.exe";

    qDebug() << "Launching Zoo Tycoon 1 from path: " << _path;

    // has proc started
    QProcess proc;
    if (!QFile::exists(_path)) {
        qWarning() << "Zoo Tycoon 1 binary not found: " << _path;
        return 0;
    }
    
    // if iso mounting enabled, mount the iso first
    if (m_settings->useIsoMounting()) {
        QString isoPath = m_settings->isoPath();
        if (!QFile::exists(isoPath)) {
            qWarning() << "ISO file not found: " << isoPath;
            return 0;
        }
        // mount the iso
        if (!PSystemMgr::mountIsoIfNeeded(isoPath)) {
            qWarning() << "Failed to mount ISO file: " << isoPath;
            return 0;
        }

        qDebug() << "ISO mounted successfully: " << isoPath;
    }
    else {
        qDebug() << "ISO mounting is disabled, skipping mount operation";
    }

    // start proc
    if (proc.startDetached(_path)) {
        qDebug() << "Zoo Tycoon 1 binary started successfully: " << _path;
        return 1;
    } else {
        qWarning() << "Failed to start Zoo Tycoon 1 binary: " << _path;
        return 0;
    }

    return 0;
}

void PState::setGamePath(QString path) {
    m_path = path;
    emit pathChanged();
}

QString PState::getGamePath() {
    return m_path;
}

QStringList PState::getZtdList() {
    // Get list of ztd files in m_resource_path
    QDir dir(m_resource_path);
    QStringList filters;
    filters << "*.ztd";

    // Get ztd paths
    QStringList ztdList;
    for (const QString &file : dir.entryList(filters, QDir::Files, QDir::Name)) {
        ztdList << dir.absoluteFilePath(file);
    }

    // Print list of ztd files
    for (const QString &ztd : ztdList) {
        qDebug() << ztd;
    }

    return ztdList;
}

void PState::loadZooIni() {
    // Load the Zoo Tycoon config file
    m_zooConfig = new PZooConfig(this, m_path + "\\zoo.ini");
    if (!m_zooConfig) {
        qDebug() << "Failed to load Zoo Tycoon config file.";
    } else {
        m_zooConfig->loadConfig();
        qDebug() << "Loaded Zoo Tycoon config file.";
    }
}

// QString configPath = m_configPath + "/config.toml";
// toml::table config = PConfigMgr::getConfig(configPath);
// if (config.empty()) {
//     // generate default settings
//     m_useIsoMounting = false;
//     m_isoPath = "";
//     // Save default settings to the TOML file
//     toml::table defaultConfig;
//     defaultConfig.insert_or_assign("zooGamePath", m_zooGamePath.toStdString());
//     defaultConfig.insert_or_assign("isoPath", "");
//     defaultConfig.insert_or_assign("useIsoMounting", false);

//     // Save the default config to the file
//     if (!PConfigMgr::saveConfig(configPath, defaultConfig)) {
//         qDebug() << "Failed to save default settings to config.toml";
//         return false;
//     }

//     qDebug() << "Default settings saved to config.toml";

//     // replace the config with the default one
//     config = defaultConfig;
// }

// // Load settings from the TOML file
// m_zooGamePath = PConfigMgr::getKeyValue("zooGamePath", config);
// m_useIsoMounting = PConfigMgr::getBoolValue("useIsoMounting", config);
// m_isoPath = PConfigMgr::getKeyValue("isoPath", config);

// // save the loaded config to the backup
// m_pandaBackupConfig = config;
// m_dirty = 0;
// m_pandaConfig = config;

// return true;

bool PState::loadPandaCfg() {
    // Load settings from the TOML file
    if (!m_pandacfg->loadFromToml()) {
        qDebug() << "Failed to load settings from config.toml";
        return false;
    } else {
        m_path = m_settings->zooGamePath();
        m_resource_path = m_settings->zooGamePath() + "\\dlupdate\\";
        qDebug() << "Loaded settings from config.toml.";
    }
    return true;
}