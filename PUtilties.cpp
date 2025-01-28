#include "PUtilties.h"

namespace {
    QMutex mutex;
}

namspace PUtilities
{
    int launchZT(const QString &path) {
        QMutexLocker locker(&mutex);

        // has proc started
        QProcess proc;
        if (!QFile::exists(path)) {
            qWarning() << "Zoo Tycoon 1 binary not found: " << path;
            return 0;
        }

        // start proc
        if (proc.startDetached(path)) {
            return 1;
        } else {
            qWarning() << "Failed to start Zoo Tycoon 1 binary: " << path;
            return 0;
        }
    }
}