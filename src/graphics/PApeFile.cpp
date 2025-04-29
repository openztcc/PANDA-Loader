#include "PApeFile.h"

PApeFile::PApeFile(const QString &ztdPath, const QString &outputDir) : m_ztdPath(ztdPath), m_outputDir(outputDir) {
    m_ztdFile = QSharedPointer<PFile>(nullptr, ztdPath);
    if (!m_ztdFile->exists()) {
        qDebug() << "ZTD file does not exist:" << ztdPath;
    }
}

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

// Process graphic buffers into cached PNG files
// returns paths to the generated PNG files
QString PApeFile::generateGraphicAsPng(const QString &graphicPath, const QString &fileName) {
    QByteArray graphicData = m_ztdFile->read(graphicPath)->data;

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
        qDebug() << "Temp file created:" << filename;
        return filename;
    };

    // Create a temp file that will store the graphic data
    QString graphicFile = createTempFile(graphicData, fileName);

    // Read Header from the graphic file
    Header header = ApeCore::getHeader(graphicFile.toStdString());
    if (header.palName.data() == nullptr) {
        qDebug() << "Invalid or missing palette path for graphic:" << graphicPath;
        return {};
    }

    // Grab the palette file rel path from the header
    QString paletteTrimmed = QString::fromLatin1(header.palName).trimmed();
    QStringList palettePath = paletteTrimmed.split('\u0000'); // clean up weird characters
    qDebug() << "Palette path: " << palettePath[0];

    // Get the palette data from the ztd file
    QByteArray paletteData = m_ztdFile->read(palettePath[0])->data;
    if (paletteData.isEmpty()) {
        qDebug() << "Failed to read palette data from ztd file:" << palettePath[0];
        return {};
    }

    // Create a temp file for the palette data
    QString paletteFile = createTempFile(paletteData, palettePath[0]);
    if (paletteFile.isEmpty()) return "";

    // read the graphic data
    ApeCore graphic;
    if (graphic.load(graphicFile.toStdString(), 0, paletteFile.toStdString()) != 1) {
        qDebug() << "Failed to load graphic: " << fileName;
        return "";
    }

    OutputBuffer** buffer = graphic.apeBuffer();
    if (!buffer || !*buffer) {
        qDebug() << "Invalid output buffer for:" << fileName;
        return "";
    }

    // add the graphic buffer to the map
    OutputBuffer* out = *buffer;
    if (out) {
        // create the PNG file from the graphic buffer
        QString  outputPng = m_outputDir + "/" + fileName + ".png";
        if (ApeCore::exportToPNG(outputPng.toStdString(), *out) == 1) {
            return outputPng;
        }
    } else {
        qDebug() << "No buffers allocated.";
    }
    return "";
}

// Deletes the icons from filesystem
bool PApeFile::deleteIcons(const QString &modId) {
    // Get the home path
    QString homePath = QDir::homePath() + "/.panda/modicons/";
    QDir dir(homePath);
    if (!dir.exists()) {
        qDebug() << "Icons directory does not exist:" << homePath;
        return false;
    }

    // Get the icon paths from database
    PDatabase db;
    db.openDatabase();
    QStringList iconPaths = db.getModByPk(modId)->iconpaths();
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
        if (QFile::exists(iconPath)) {
            if (!QFile::remove(iconPath)) {
                qDebug() << "Failed to delete icon file:" << iconPath;
                return false;
            } else {
                qDebug() << "Deleted icon file:" << iconPath;
            }
        } else {
            qDebug() << "Icon file does not exist:" << iconPath;
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
