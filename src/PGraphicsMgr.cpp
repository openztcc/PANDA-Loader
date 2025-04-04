#include "PGraphicsMgr.h"

PGraphicsMgr::PGraphicsMgr() {}

// Get the graphic buffers from a ztd file
QMap<QString, OutputBuffer> PGraphicsMgr::getGraphicBuffers(const QString &ztdFilePath) {
    QMap<QString, OutputBuffer> graphicBuffers;
    QStringList graphicPaths = PConfigMgr::getIconPaths(ztdFilePath);
    std::vector<std::unique_ptr<QTemporaryFile>> tempFiles;


    // local function to create temp files
    auto createTempFile = [&](const QByteArray &fileData, const QString &filePath) -> QString {
        auto tempFile = std::make_unique<QTemporaryFile>();
        tempFile->setAutoRemove(true);
        if (!tempFile->open()) {
            qDebug() << "Failed to create temp file for:" << filePath;
            return {};
        }
        tempFile->write(fileData);
        tempFile->flush();

        QString filename = tempFile->fileName();
        tempFiles.push_back(std::move(tempFile));
        qDebug() << "Temp file created:" << filename;
        return filename;
    };

    
    for (const QString &path : graphicPaths) {
        // make temp file for the graphic
        QByteArray graphicData = PZtdMgr::getFileFromRelPath(ztdFilePath, path);
        if (graphicData.isEmpty()) {
            qDebug() << "Graphic data is empty for path:" << path;
            continue;
        }
        QString graphicFile = createTempFile(graphicData, path);
        if (graphicFile.isEmpty()) {
            qDebug() << "Failed to write graphic data to temp file:" << path;
            continue;
        }
        if (!QFile::exists(graphicFile)) {
            qDebug() << "Temp file was not created:" << graphicFile;
            continue;
        }
        QString projectName = path.split('/')[1];

        Header header = ApeCore::getHeader(graphicFile.toStdString());
        if (header.palName.data() == nullptr){
            qDebug() << "Invalid or missing palette path for graphic:" << path;
            continue;
        }

        QString paletteTrimmed = QString::fromLatin1(header.palName).trimmed();
        QStringList palettePath = paletteTrimmed.split('\u0000');
        qDebug() << "Palette path: " << palettePath[0];
        QByteArray paletteData = PZtdMgr::getFileFromRelPath(ztdFilePath, palettePath[0]);
        QString paletteFile = createTempFile(paletteData, palettePath[0]);
        if (paletteFile.isEmpty()) continue;

        // read the graphic data
        ApeCore graphic;
        if (graphic.load(graphicFile.toStdString(), 0, paletteFile.toStdString()) != 1) {
            qDebug() << "Failed to load graphic: " << path;
            continue;
        }

        OutputBuffer** buffer = graphic.apeBuffer();
        if (!buffer || !*buffer) {
            qDebug() << "Invalid output buffer for:" << path;
            continue;
        }

        // add the graphic buffer to the map
        QString graphicName = path.section('/', -2, -2);
        OutputBuffer* out = *buffer;
        if (out) {
            graphicBuffers.insert(graphicName + '_' + projectName, *out);
        } else {
            qDebug() << "No buffers allocated.";
        }

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

        if (ApeCore::exportToPNG(pngPath.toStdString(), it.value()) == 1) {
            pngPaths.append(pngPath);
        } else {
            qDebug() << "Failed to export PNG: " << pngPath;
        }
    }
    return pngPaths;
}
