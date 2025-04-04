#include "PGraphicsMgr.h"

PGraphicsMgr::PGraphicsMgr() {}

// Get the graphic buffers from a ztd file
QMap<QString, OutputBuffer> PGraphicsMgr::getGraphicBuffers(const QString &ztdFilePath) {
    QMap<QString, OutputBuffer> graphicBuffers;
    QStringList graphicPaths = PConfigMgr::getIconPaths(ztdFilePath);

    // local function to create temp files
    auto createTempFile = [](const QByteArray &fileData, const QString &filePath) {
        QTemporaryFile tempFile;
        tempFile.setAutoRemove(true);
        if (!tempFile.open()) {
            qDebug() << "Failed to create temp file: " << filePath;
            return QString();
        }

        tempFile.write(fileData);
        tempFile.flush();

        return tempFile.fileName();
    };
    
    for (const QString &path : graphicPaths) {
        // make temp file for the graphic
        QByteArray graphicData = PZtdMgr::getFileFromRelPath(ztdFilePath, path);
        QString graphicFile = createTempFile(graphicData, path);
        if (graphicFile.isEmpty()) {
            qDebug() << "Failed to write graphic data to temp file:" << path;
            continue;
        }
        QString projectName = path.section("/", 0, -2).toUpper();

        // read the graphic header
        Header header = ApeCore::getHeader(graphicFile.toStdString());
        if (header.palName.empty()){
            qDebug() << "Failed to get palette path from header for:" << path;
            continue;
        }

        // get palette file
        QString palettePath = QString::fromUtf8(header.palName.data(), static_cast<int>(header.palName.size())); // converts to QString from char vector
        if (palettePath.isEmpty()) {
            qDebug() << "Failed to get palette path for graphic: " << path;
            continue;
        }

        // grab palette file from ztd
        QByteArray paletteData = PZtdMgr::getFileFromRelPath(ztdFilePath, palettePath);
        // create temp file for the palette
        QString paletteFile = createTempFile(paletteData, palettePath);

        // read the graphic data
        ApeCore graphic;
        if (graphic.load(graphicFile.toStdString(), 0, paletteFile.toStdString()) != 0) {
            qDebug() << "Failed to load graphic: " << path;
            continue;
        }

        OutputBuffer** buffer = graphic.apeBuffer();
        if (!buffer || !*buffer) {
            qDebug() << "Invalid output buffer for:" << path;
            continue;
        }

        // add the graphic buffer to the map
        QString graphicName = path.section("/", -1, -1).toUpper();
        graphicBuffers.insert(graphicName + '_' + projectName, *buffer[0]);
    }

    return graphicBuffers;
}

// Process graphic buffers into cached PNG files
// returns paths to the generated PNG files
QStringList PGraphicsMgr::processIcons(QMap<QString, OutputBuffer> &graphicBuffers) {
    QStringList pngPaths;
    for (auto it = graphicBuffers.begin(); it != graphicBuffers.end(); ++it) {
        QString graphicName = it.key();
        QString homePath = QDir::homePath() + "/.panda/modicons/";
        QString pngPath = homePath + graphicName + ".png";

        if (ApeCore::exportToPNG(pngPath.toStdString(), it.value()) == 0) {
            pngPaths.append(pngPath);
        } else {
            qDebug() << "Failed to export PNG: " << pngPath;
        }
    }
    return pngPaths;
}
