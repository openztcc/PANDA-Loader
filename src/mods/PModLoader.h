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

    QVector<QSharedPointer<PModItem>> buildCollectionMods(const QVector<QSharedPointer<PFileData>> &entryPoints, QSharedPointer<PModItem> &mod, PFile &ztd);
    QSharedPointer<PModItem> buildModFromToml(PConfigMgr &config);
    QSharedPointer<PModItem> buildDefaultMod(const QString &ztdPath);
    void generateFileData(const QString &filePath, QSharedPointer<PModItem> &mod);
    QStringList generateTagsFromConfig(const QSharedPointer<PConfigMgr> &config);
    QString determineCategory(const QSharedPointer<PFileData> &fileData);
    QStringList getIconPngPaths(const QSharedPointer<PConfigMgr> &config, const QSharedPointer<PFileData> &entryPoint, const QString &category, PFile &ztd);
    QMap<QString, QString> getIconAniPaths(const QSharedPointer<PConfigMgr> &config, const QString &category);
    QStringList getIconPaths(const QStringList &aniPaths, const QSharedPointer<PFile> &ztd,
        const QSharedPointer<PConfigMgr> &entryPointConfig, const QString &category);
    QString buildGraphicPath(const QSharedPointer<PConfigMgr> &aniFile);
    QString buildIconFileName(const QSharedPointer<PConfigMgr> &entryPointConfig, const QString &category, const QString &aniFileName);

private:
    QSharedPointer<PModDataAccess> m_dataAccess;

};

#endif // PMODLOADER_H