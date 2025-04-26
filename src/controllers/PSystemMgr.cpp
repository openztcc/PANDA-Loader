#include "PSystemMgr.h"

PSystemMgr::PSystemMgr() {}

bool PSystemMgr::isIsoMounted(const QString &isoPath)
{
    QProcess process;
    process.start("powershell", QStringList() 
        << "-command" 
        << QString("Get-DiskImage -ImagePath '%1' | Select-Object -ExpandProperty Attached").arg(isoPath));
    process.waitForFinished();
    
    QString output = process.readAllStandardOutput().trimmed();
    return (output.toLower() == "true");
}

bool PSystemMgr::mountIso(const QString& isoPath, QString& mountPoint) {
    QProcess process;
    
    #ifdef Q_OS_WIN
        // windows (Powershell): using Mount-DiskImage
        mountPoint = ""; // note: windows assigns mount point automatically (drive letter)
        process.start("powershell", QStringList() 
            << "-command" 
            << QString("Mount-DiskImage -ImagePath '%1' -PassThru | Get-Volume").arg(isoPath));
    
    #elif defined(Q_OS_MACOS)
        // macOS: using hdiutil
        mountPoint = "/Volumes/" + QFileInfo(isoPath).baseName(); // Approximate mount point
        process.start("hdiutil", QStringList() << "attach" << isoPath);
        
    #else
        // Linux: using mount
        // TODO: might need to check for existing mount points
        mountPoint = "/mnt/iso"; // might want this configurable?
        QDir().mkpath(mountPoint); // check if mount point exists, create if not
        process.start("sudo", QStringList() << "mount" << "-o" << "loop" << isoPath << mountPoint);
    #endif
    
    process.waitForFinished();
    return (process.exitCode() == 0);
}

bool PSystemMgr::mountIsoIfNeeded(const QString &isoPath)
{
    if (PSystemMgr::isIsoMounted(isoPath)) {
        qDebug() << "ISO is already mounted, skipping mount operation";
        // TODO: get  drive letter of already mounted ISO
        // QString driveLetter = getDriveLetter(isoPath);
    } 
    else {
        // Proceed with mounting
        QString mountPoint;
        if (PSystemMgr::mountIso(isoPath, mountPoint)) {
            qDebug() << "Successfully mounted ISO to" << mountPoint;
        } 
        else {
            qDebug() << "Failed to mount ISO";
            return false;
        }
    }

    return true;
}