#include "PState.h"

PState::PState(QObject *parent) : QObject(parent) {
    m_path = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\zoo.exe";
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
