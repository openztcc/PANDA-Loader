#include "PApeFile.h"

PApeFile::PApeFile(const QSharedPointer<PFile> &ztd, const QString &outputDir) : m_ztd(ztd), m_outputDir(outputDir) {
    if (!m_ztd) {
        qDebug() << "Tried to access a null ztd file pointer in PApeFile.";
        return;
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
    qDebug() << "Generating graphic as PNG:" << graphicPath << "with file name:" << fileName;
    QByteArray graphicData = m_ztd->read(graphicPath)->data;

    qDebug() << "Graphic data size:" << graphicData.size();

    // local function to create temp files
    auto createTempFile = [&](const QByteArray &fileData, const QString &filePath) -> QString {
        QTemporaryFile *tempFile = new QTemporaryFile();
        tempFile->setAutoRemove(false);
        if (tempFile->open()) {
            tempFile->write(fileData);
            tempFile->flush();
            return tempFile->fileName();
        } else {
            qDebug() << "Failed to create temp file:" << tempFile->errorString();
            return "";
        }
    };

    // Create a temp file that will store the graphic data
    QString graphicFile = createTempFile(graphicData, fileName);

    // Read Header from the graphic file
    Header header = ApeCore::getHeader(graphicFile.toStdString());
    qDebug() << "Header size:" << sizeof(header);
    if (header.palName.size() == 0) {
        qDebug() << "Invalid or missing palette path for graphic:" << graphicPath;
        QFile::remove(graphicFile);
        return {};
    }

    // Grab the palette file rel path from the header
    QString paletteTrimmed = QString::fromLatin1(header.palName).trimmed();
    QStringList palettePath = paletteTrimmed.split('\u0000'); // clean up weird characters
    qDebug() << "Palette path: " << palettePath[0];

    // Get the palette data from the ztd file
    QSharedPointer<PFileData> paletteFile = m_ztd->read(palettePath[0]);
    if (!paletteFile) {
        qDebug() << "Failed to read palette file:" << palettePath[0];
        QFile::remove(graphicFile);
        return "";
    }
    QByteArray paletteData = paletteFile->data;
    // Create a temp file for the palette data
    QString palTempFile = createTempFile(paletteData, palettePath[0]);
    if (palTempFile.isEmpty())
    {
        QFile::remove(graphicFile);
        return "";
    }

    // read the graphic data
    ApeCore graphic;
    if (graphic.load(graphicFile.toStdString(), 0, palTempFile.toStdString()) != 1) {
        qDebug() << "Failed to load graphic: " << fileName;
        QFile::remove(graphicFile);
        return "";
    }

    OutputBuffer** buffer = graphic.apeBuffer();
    if (!buffer || !*buffer) {
        qDebug() << "Invalid output buffer for:" << fileName;
        QFile::remove(graphicFile);
        return "";
    }

    // add the graphic buffer to the map
    OutputBuffer* out = *buffer;
    if (out) {
        // create the PNG file from the graphic buffer
        QString  outputPng = m_outputDir + "/" + fileName + ".png";
        if (ApeCore::exportToPNG(outputPng.toStdString(), *out) == 1) {
            // cleanup
            QFile::remove(graphicFile);
            return outputPng;
        }
    } else {
        qDebug() << "No buffers allocated.";
    }
    // cleanup
    QFile::remove(graphicFile);
    return "";
}
