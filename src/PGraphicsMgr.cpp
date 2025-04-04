#include "PGraphicsMgr.h"

PGraphicsMgr::PGraphicsMgr() {}

PGraphicsMgr::~PGraphicsMgr() {}

// Get the graphic buffers from a ztd file
std::vector<OutputBuffer> PGraphicsMgr::getGraphicBuffers(const QString &ztdFilePath) {
    std::vector<OutputBuffer> buffers;
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

        for (int i = 0; i < graphic.getFrameCount(); ++i) {
            OutputBuffer output = buffer[i];
            buffers.push_back(output);
        }
        graphicFile.close();
        paletteFile.close();

        // cleanup temp files
        QFile::remove(graphicFile.fileName());
        QFile::remove(paletteFile.fileName());

    }
}