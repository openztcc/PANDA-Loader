#ifndef PMODDAL_H
#define PMODDAL_H

// This class is the layer between the QML and C++. It is responsible for
// communicating data between the mod loader and the database.
// The mod loader structure is as such:

// - Database, Files, and Configs
// - PModDAL, Model
// - PModLoader < ---- facilitates model and db
// - QML

// Qt
#include <QCore>

// Project
#include "PModItem.h"
#include "PDatabase.h"

class PModDAL {
    public:
        PModDAL(QObject *parent = nullptr);
        ~PModDAL();
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
    private:
        PDatabase m_db;
        const QString m_tableName = "mods";
        const QString m_createTableQuery = 
            "CREATE TABLE IF NOT EXISTS mods ("
            "pk INTEGER PRIMARY KEY AUTOINCREMENT, "
            "title TEXT NOT NULL, "
            "authors TEXT NOT NULL, "
            "description TEXT, "
            "enabled INTEGER NOT NULL, "
            "category TEXT, "
            "tags TEXT, "
            "version TEXT NOT NULL, "
            "mod_id TEXT NOT NULL UNIQUE, "
            "dep_id TEXT, "
            "iconpaths TEXT, "
            "filename TEXT, "
            "location TEXT, "
            "oglocation TEXT, "
            "is_selected INTEGER NOT NULL, "
            "FOREIGN KEY(mod_id) REFERENCES dependencies(mod_id)"
            ");";

        const QString m_createDependenciesTableQuery =
            "CREATE TABLE IF NOT EXISTS dependencies ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "mod_id TEXT NOT NULL, "
            "dependency_id TEXT NOT NULL, "
            "name TEXT NOT NULL, "
            "min_version TEXT, "
            "optional INTEGER NOT NULL, "
            "ordering TEXT, "
            "link TEXT, "
            "FOREIGN KEY(mod_id) REFERENCES mods(mod_id)"
            ");";

        const QString m_insertModQuery = 
            "INSERT INTO mods (title, authors, description, enabled, category, tags, version, mod_id, dep_id, filename, location, iconpaths, oglocation, is_selected) "
            "VALUES (:title, :authors, :description, :enabled, :category, :tags, :version, :mod_id, :dep_id, :filename, :location, :iconpaths, :oglocation, :is_selected)";
};

#endif // PModDAL_H