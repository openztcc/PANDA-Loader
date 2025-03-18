#include "PState.h"
#include <qdir.h>

PState::PState(QObject *parent) : QObject(parent) {
    m_path = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\zoo.exe";
    m_resource_path = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\dlupdate\\";
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

