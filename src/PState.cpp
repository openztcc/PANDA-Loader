#include "PState.h"
#include <qdir.h>

PState::PState(QObject *parent) : QObject(parent) {
    m_path = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\";
    m_resource_path = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\dlupdate\\";
    m_mods = QVector<PModItem>();
    m_settings = new PSettings(this);
    // Load settings from the TOML file
    if (!m_settings->loadFromToml()) {
        qDebug() << "Failed to load settings from config.toml";
    } else {
        m_path = m_settings->zooGamePath();
        m_resource_path = m_settings->zooGamePath() + "\\dlupdate\\";
        qDebug() << "Loaded settings from config.toml.";
    }

    // Load the Zoo Tycoon config file
    m_zooConfig = new PZooConfig(this, m_path + "zoo.ini");
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

