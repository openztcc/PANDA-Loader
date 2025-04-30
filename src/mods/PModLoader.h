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
#include "PModDataAccess.h"

class PModLoader : public QObject
{
    Q_OBJECT
public:
    explicit PModLoader(QObject *parent = nullptr, QSharedPointer<PModDataAccess> dataAccess = nullptr)
        : QObject(parent), m_dataAccess(dataAccess) {}
    ~PModLoader() override = default;

    // Load mods from a given directory
    Q_INVOKABLE void loadModsFromDirectory(const QString &directoryPath);

    // Load mods from a given file
    Q_INVOKABLE void loadModsFromFile(const QStringList &filePath);

private:

    // Mod loaders
    QVector<QSharedPointer<PModItem>> buildCollectionMods(const QVector<QSharedPointer<PFileData>> &entryPoints, const QSharedPointer<PModItem> &mod, const QSharedPointer<PFile> &ztd);
    QSharedPointer<PModItem> buildModFromToml(PConfigMgr &config);
    QSharedPointer<PModItem> buildDefaultMod(const QString &ztdPath);

    // File data
    void generateFileData(const QString &filePath, QSharedPointer<PModItem> &mod);

    // Categorization data
    QStringList generateTagsFromConfig(const QSharedPointer<PConfigMgr> &config);

    // Icon data
    QString determineCategory(const QSharedPointer<PFileData> &fileData);
    QStringList getIconPngPaths(const QSharedPointer<PConfigMgr> &config, const QSharedPointer<PFileData> &entryPoint, const QString &category, const QSharedPointer<PFile> &ztd);
    QMap<QString, QString> getIconAniPaths(const QSharedPointer<PConfigMgr> &config, const QString &category);
    QStringList getIconPaths(const QMap<QString, QString> &aniPaths, const QSharedPointer<PFile> &ztd, const QString &typeName);
    QString buildGraphicPath(const QSharedPointer<PConfigMgr> &aniFile);

    // Description and tit
    QString getDescription(const QSharedPointer<PConfigMgr> &config, const QString &category);
    QString getTitle(const QSharedPointer<PConfigMgr> &config, const QString &category);

    // Mod Cleanup
    bool deleteIcons(const QString &modId);

    // Private fields
    QSharedPointer<PModDataAccess> m_dataAccess;

};

#endif // PMODLOADER_H