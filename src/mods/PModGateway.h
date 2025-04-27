#ifndef PMODGATEWAY_H
#define PMODGATEWAY_H

// This class is the layer between the QML and C++. It is responsible for
// communicating data between the mod loader and the database.
// The mod loader structure is as such:

// - Database, Files, and Configs
// - PModGateway, Model
// - PModLoader < ---- facilitates model and db
// - QML

// Qt
#include <QCore>

// Project
#include "PModItem.h"
#include "PDatabase.h"

class PModGateway {
    public:
        PModGateway(QObject *parent = nullptr);
        ~PModGateway();
        bool insertMod(const QString &title, const QString &description, const QStringList &authors, 
            const QString &version, bool enabled, const QStringList &tags, const QString &category, 
            const QString &id, const QString &depId, const QString &filename,
            const QString &location, const QStringList &iconpaths, const QString &oglocation,
            bool selected, QObject *parent);
        bool insertMod(const PModItem &mod);
        bool deleteMod(const QString &modId);
        bool updateMod(const QString &modId, const QString &key, const QString &value);
        bool addDependency(const QString &modId, const PDependency &dependency);
        bool removeDependency(const QString &depId);
        bool doesModExist(const QString &modId);
        bool doesDependencyExist(const QString &dependencyId);
        QSqlQuery getAllMods();
        QSqlQuery queryMods(const QString &propertyName, const QString &searchTerm);
        QStringList searchMods(const QString &propertyName, const QString &searchTerm);
        Q_INVOKABLE QSharedPointer<PModItem> getModByPk(const QString &modId);
        QSharedPointer<PModItem> populateModItem(QSqlQuery &query);
        void loadModsFromZTDs(const QStringList &ztdList);
        QSharedPointer<PModItem> queryToModItem(QSqlQuery &query);
        QSharedPointer<PModItem> queryToModItem(QString property, QString value);
        QVector<QSharedPointer<PModItem>> queryToModItems(QString property, QString value);
};

#endif // PMODGATEWAY_H