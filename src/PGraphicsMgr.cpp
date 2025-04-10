#include "PGraphicsMgr.h"

PGraphicsMgr::PGraphicsMgr() {}

OutputBuffer outputBufferCopy(const OutputBuffer& src) {
    OutputBuffer copy;
    copy.width = src.width;
    copy.height = src.height;
    copy.channels = src.channels;
    copy.offsetX = src.offsetX;
    copy.offsetY = src.offsetY;

    size_t bufferSize = static_cast<size_t>(src.width) * src.height * src.channels;
    copy.pixels = new uint8_t[bufferSize];
    std::memcpy(copy.pixels, src.pixels, bufferSize);

    return copy;
}


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
        // check if files already exist 
        QString projectName = path.split('/')[1];
        QString graphicName = path.section('/', -2, -2);
        QString homePath = QDir::homePath() + "/.panda/modicons/";
        QString fullPath = homePath + graphicName + '_' + projectName + ".png";
        if (QFile::exists(fullPath)) {
            // qDebug() << "Graphic already exists:" << fullPath;
            break;
        }
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
        OutputBuffer* out = *buffer;
        if (out) {
            graphicBuffers.insert(graphicName + '_' + projectName, outputBufferCopy(*out));
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
        QString fileUrl = QUrl::fromLocalFile(pngPath).toString();

        // check if file already exists
        if (QFile::exists(pngPath)) {
            qDebug() << "PNG file already exists:" << pngPath;
            continue;
        }
        // create the directory if it doesn't exist
        QDir dir(homePath);
        if (!dir.exists()) {
            if (!dir.mkpath(homePath)) {
                qDebug() << "Failed to create directory:" << homePath;
                continue;
            }
        }

        if (ApeCore::exportToPNG(pngPath.toStdString(), it.value()) == 1) {
            pngPaths.append(fileUrl);
        } else {
            qDebug() << "Failed to export PNG: " << graphicName;
        }
    }
    return pngPaths;
}

// Deletes the icons from filesystem
bool PGraphicsMgr::deleteIcons(const QString &modId) {
    // Get the home path
    QString homePath = QDir::homePath() + "/.panda/modicons/";
    QDir dir(homePath);
    if (!dir.exists()) {
        qDebug() << "Icons directory does not exist:" << homePath;
        return false;
    }

    // Get the icon paths from database
    PDatabaseMgr db;
    db.openDatabase();
    QStringList iconPaths = db.getModByPk(modId).iconpaths;
    db.closeDatabase();

    if (iconPaths.isEmpty()) {
        qDebug() << "No icon paths found for mod ID:" << modId;
        // print the paths for debugging
        for (const QString &iconPath : iconPaths) {
            qDebug() << "Icon path:" << iconPath;
        }
        return false;
    }

    // Delete each icon file
    for (const QString &iconPath : iconPaths) {
        // Convert QUrl to local file path
        QString localPath = QUrl(iconPath).toLocalFile();
        if (QFile::exists(localPath)) {
            if (!QFile::remove(localPath)) {
                qDebug() << "Failed to delete icon file:" << localPath;
                return false;
            } else {
                qDebug() << "Deleted icon file:" << localPath;
            }
        } else {
            qDebug() << "Icon file does not exist:" << localPath;
        }
    }

    // Remove the directory if empty
    if (dir.isEmpty()) {
        if (!dir.rmdir(homePath)) {
            qDebug() << "Failed to remove empty directory:" << homePath;
            return false;
        } else {
            qDebug() << "Removed empty directory:" << homePath;
        }
    } else {
        qDebug() << "Directory is not empty, not removing:" << homePath;
    }

    return true;
}
