#include "PGraphicsMgr.h"

PGraphicsMgr::PGraphicsMgr() {}

PGraphicsMgr::~PGraphicsMgr() {}

// Get the graphic buffers from a ztd file
QMap<QString, OutputBuffer> PGraphicsMgr::getGraphicBuffers(const QString &ztdFilePath) {
    QMap<QString, OutputBuffer> graphicBuffers;
    QStringList graphicPaths = PConfigMgr::getIconPaths(ztdFilePath);
    for (const QString &path : graphicPaths) {
        // make temp file for the graphic
        QTempFile graphicFile(path);
        if (!graphicFile.open()) {
            qDebug() << "Failed to open temp file for graphic: " << path;
            continue;
        }

        // read the graphic header
        QByteArray header = ApeCore::getFileHeader(graphicFile.fileName());
        if (header.isEmpty()) {
            qDebug() << "Failed to get file header for graphic: " << path;
            continue;
        }

        // get palette file
        QString palettePath = header->palName;
        if (palettePath.isEmpty()) {
            qDebug() << "Failed to get palette path for graphic: " << path;
            continue;
        }

        // create temp file for the palette
        QTempFile paletteFile(palettePath);
        if (!paletteFile.open()) {
            qDebug() << "Failed to open temp file for palette: " << palettePath;
            continue;
        }

        // read the graphic data
        ApeCore graphic;
        if (graphic.load(graphicFile.fileName(), 0, paletteFile.fileName()) != 0) {
            qDebug() << "Failed to load graphic: " << path;
            continue;
        }

        OutputBuffer** buffer = graphic.getOutputBuffer();
        if (buffer == nullptr) {
            qDebug() << "Failed to get output buffer for graphic: " << path;
            continue;
        }

        // add the graphic buffer to the map
        QString graphicName = path.section("/", -1, -1).toUpper();
        graphicBuffers.insert(graphicName, *buffer);
        
        graphicFile.close();
        paletteFile.close();

        // cleanup temp files
        QFile::remove(graphicFile.fileName());
        QFile::remove(paletteFile.fileName());

    }
}

// Process graphic buffers into cached PNG files
// returns paths to the generated PNG files
QStringList PGraphicsMgr::processIcons(QMap<QString, OutputBuffer> &graphicBuffers) {
    QStringList pngPaths;
    for (auto it = graphicBuffers.begin(); it != graphicBuffers.end(); ++it) {
        QString pngPath = it.key() + ".png";
        if (ApeCore::exportToPNG(pngPath, it.value()) == 0) {
            pngPaths.append(pngPath);
        } else {
            qDebug() << "Failed to export PNG: " << pngPath;
        }
    }
    return pngPaths;
}