#include "PState.h"
#include <qdir.h>

PState::PState(QObject *parent) : QObject(parent) {
    m_path = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\";
    m_resource_path = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\dlupdate\\";
    m_mods = QVector<PModItem>();
    loadPandaCfg();
    // Load settings from the TOML file
    if (!m_pandacfg) {
        qDebug() << "Failed to load settings from config.toml";
    } else {
        m_path =  m_pandacfg->getValue("", "zooGamePath").toString();
        m_resource_path = m_path + "\\dlupdate\\";
        qDebug() << "Loaded settings from config.toml.";
    }

    loadZooIni();
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
    bool useISOMountng = m_pandacfg->getValue("", "useIsoMounting").toBool();
    if (useISOMountng) {
        QString isoPath = m_pandacfg->getValue("", "isoPath").toString();
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

bool PState::loadZooIni() {
    // Load the Zoo Tycoon config file
    m_zooini = new PConfigMgr(this, m_path + "\\zoo.ini");
    if (!m_zooini) {
        qDebug() << "Failed to load Zoo Tycoon config file.";
        return false;
    }
    return true;
}

bool PState::loadPandaCfg() {
    // panda config file path
    QString configPath = m_configPath + "/panda.toml";
    // check if the config file exists
    if (!QFile::exists(configPath)) {
        qDebug() << "Panda config file not found: " << configPath;
        // time to create a new one
        PConfigMgr config(this, configPath);

        // add default settings to the config
        config.setValue("zooGamePath", m_path, "");
        config.setValue("useIsoMounting", false, "");
        config.setValue("isoPath", "", "");

        // replace the config with the default one
        m_pandacfg = &config;
        m_pandacfg->saveConfig(configPath);
    } else {
        // load the config file
        m_pandacfg = new PConfigMgr(this, configPath);
        if (!m_pandacfg->loadConfig(configPath)) {
            qDebug() << "Failed to load panda config file: " << configPath;
            return false;
        }
    }
}
