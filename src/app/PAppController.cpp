#include "PAppController.h"
#include <qdir.h>

PAppController::PAppController(QObject *parent) : QObject(parent) {
    m_path = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\";
    m_resource_path = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\dlupdate\\";
    m_mods = QVector<PModItem>();
    m_dirty = 0;
    m_zooiniDirty = 0;
    m_pandacfgDirty = 0;
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

    // this connects the worker PConfigMgr objects to this manager PAppController object so that there
    // is one "source of truth" for the dirty flag
    // TODO: these connections work but PAppController slot needs fixed
    connect(m_zooini.get(), &PConfigMgr::dirtyChanged, this, &PAppController::onConfigDirtyChanged);
    connect(m_pandacfg.get(), &PConfigMgr::dirtyChanged, this, &PAppController::onConfigDirtyChanged);
}

int PAppController::launchZT() {
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
        if (!PLauncher::mountIsoIfNeeded(isoPath)) {
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

void PAppController::setGamePath(QString path) {
    m_path = path;
    emit pathChanged();
}

QString PAppController::getGamePath() {
    return m_path;
}

bool PAppController::loadZooIni() {
    // Load the Zoo Tycoon config file
    m_zooini = QSharedPointer<PConfigMgr>::create(this, m_path + "\\zoo.ini");
    if (!m_zooini) {
        qDebug() << "Failed to load Zoo Tycoon config file.";
        return false;
    }
    return true;
}

bool PAppController::loadPandaCfg() {
    // panda config file path
    QString configPath = m_configPath + "/panda.toml";
    // check if the config file exists
    if (!QFile::exists(configPath)) {
        qDebug() << "Panda config file not found: " << configPath;
        // time to create a new one
         m_pandacfg = QSharedPointer<PConfigMgr>::create(this, configPath);

        // add default settings to the config
        m_pandacfg->setValue("zooGamePath", m_path, "");
        m_pandacfg->setValue("useIsoMounting", false, "");
        m_pandacfg->setValue("isoPath", "", "");

        m_pandacfg->saveConfig(configPath);
    } else {
        // load the config file
        m_pandacfg = QSharedPointer<PConfigMgr>::create(this, configPath);
        if (!m_pandacfg) {
            qDebug() << "Failed to load Panda config file: " << configPath;
            return false;
        }

    }
    return true;
}

// this moves the openzt binary dll to the root zt folder
int PAppController::launchScripting() {
    // source file is in libs/res-openzt.dll
    QString sourcePath = QDir::currentPath() + "/libs/res-openzt.dll";

    // destination file is in the zt folder
    QString destPath = QDir(m_path).filePath("res-openzt.dll");

    // check if the source file exists
    if (!QFile::exists(sourcePath)) {
        qWarning() << "Source file not found: " << sourcePath;
        return 0;
    }

    // check if the destination file exists
    if (QFile::exists(destPath)) {
        qWarning() << "Destination file already exists: " << destPath;
        return 0;
    }

    // copy the file
    if (!QFile::copy(sourcePath, destPath)) {
        qWarning() << "Failed to copy file: " << sourcePath << " to " << destPath;
        return 0;
    }

    // make a scripts directory in the zt folder if it doesn't exist
    QString scriptsPath = m_path + "/scripts";
    if (!QDir().exists(scriptsPath)) {
        QDir().mkdir(scriptsPath);
    }
}
