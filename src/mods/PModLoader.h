#ifndef PMODLOADER_H
#define PMODLOADER_H

#include <QObject>

// Project
#include "PModItem.h"
#include "PDatabase.h"
#include "PModQueries.h"
#include "PFileData.h"
#include "PConfigMgr.h"
#include "PApeFile.h"

class PModLoader : public QObject
{
    Q_OBJECT
public:
    explicit PModLoader(QObject *parent = nullptr);
    ~PModLoader() override = default;

    // Load mods from a given directory
    Q_INVOKABLE void loadModsFromDirectory(const QString &directoryPath);

    // Load mods from a given file
    Q_INVOKABLE void loadModsFromFile(const QStringList &filePath);

    QVector<QSharedPointer<PModItem>> buildCollectionMods(QList<PFileData> entryPoints, QSharedPointer<PModItem> mod, PFile &ztd);
    QSharedPointer<PModItem> buildModFromToml(PConfigMgr &config);
    QSharedPointer<PModItem> buildDefaultMod(const QString &ztdPath);
    void generateFileData(const QString &filePath, QSharedPointer<PModItem> mod);
    QStringList generateTagsFromConfig(PConfigMgr &config);
    QString determineCategory(const PFileData &fileData);
    QStringList getIconPngPaths(PConfigMgr &config, const QString &category, PFile &ztd);
    QStringList getIconAniPaths(PConfigMgr &config, const QString &category);
    QStringList getIconPaths(const QStringList &aniPaths, PFile &ztd);
    QString buildGraphicPath(PConfigMgr &aniFile);

};

#endif // PMODLOADER_H