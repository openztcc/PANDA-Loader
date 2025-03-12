#include "PState.h"

PState::PState(QObject *parent) : QObject(parent) {
    m_path = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\zoo.exe";
    m_mods = QVector<PModItem>();
}

int PState::launchZT() {
    // QMutexLocker locker(&mutex);
    QString _path = this->m_path;

    // has proc started
    QProcess proc;
    if (!QFile::exists(_path)) {
        qWarning() << "Zoo Tycoon 1 binary not found: " << _path;
        return 0;
    }

    // start proc
    if (proc.startDetached(_path)) {
        return 1;
    } else {
        qWarning() << "Failed to start Zoo Tycoon 1 binary: " << _path;
        return 0;
    }

    return 0;
}

void PState::setPath(QString path) {
    m_path = path;
    emit pathChanged();
}

QString PState::getPath() {
    return m_path;
}

QStringList PState::getZtdList() {
    // Get list of ztd files in m_path
    QDir dir(m_path);
    QStringList filters;
    filters << "*.ztd";
    QStringList ztdList = dir.entryList(filters, QDir::Files, QDir::Name);

    // Print list of ztd files
    for (const QString &ztd : ztdList) {
        qDebug() << ztd;
    }

    return ztdList;
}

void PState::loadMods() {
    // Scan m_path for mods
    QStringList ztdList = getZtdList();

    // Load mods into m_mods
    for (const QString &ztd : ztdList) {
        PModItem mod;
        toml::table config = PConfigMgr::getMetaConfig(ztd);
        mod.setmodTitle(PConfigMgr::getKeyValue("title", config));
        mod.setmodAuthor(PConfigMgr::getKeyValue("authors", config));
        mod.setmodDescription(PConfigMgr::getKeyValue("description", config));
        mod.setVersion(PConfigMgr::getKeyValue("version", config));
        mod.setmodPath(ztd);
        mod.setmodEnabled(true);
        mod.setmodTags(PConfigMgr::getKeyValue("tags", config));
        m_mods.append(mod);
    }

}
