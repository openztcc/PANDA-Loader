#include "PSystemMgr.h"

PSystemMgr::PSystemMgr() {}

bool PSystemMgr::isIsoMounted(const QString &isoPath) const
{
    QProcess process;
    process.start("powershell", QStringList() 
        << "-command" 
        << QString("Get-DiskImage -ImagePath '%1' | Select-Object -ExpandProperty Attached").arg(isoPath));
    process.waitForFinished();
    
    QString output = process.readAllStandardOutput().trimmed();
    return (output.toLower() == "true");
}

bool PSystemMgr::mountIso(const QString &isoPath) const
{
    if (isIsoAlreadyMounted(isoPath)) {
        qDebug() << "ISO is already mounted, skipping mount operation";
        // TODO: get  drive letter of already mounted ISO
        // QString driveLetter = getDriveLetter(isoPath);
    } else {
        // Proceed with mounting
        QString mountPoint;
        if (mountIso(isoPath, mountPoint)) {
            qDebug() << "Successfully mounted ISO to" << mountPoint;
        } else {
            qDebug() << "Failed to mount ISO";
        }
    }
}