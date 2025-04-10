#ifndef PDATABASEMGR_H
#define PDATABASEMGR_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVector>
#include <QStringList>
#include <QDir>
#include <QCoreApplication>
#include "PGraphicsMgr.h"
#include "PZtdMgr.h"

class PDatabaseMgr : public QObject
{
    Q_OBJECT
public:
    PDatabaseMgr();
    ~PDatabaseMgr();

    struct PDependency
    {
        QString dependencyId;
        QString modId;
        QString name;
        QString min_version;
        bool optional;
        QString ordering;
        QString link;
    };

    bool openDatabase();
    void closeDatabase();
    bool createTables();
    bool insertMod(const QString &name, const QString &desc, const QVector<QString> &authors,
                   const QString &version, bool enabled, const QVector<QString> &tags,
                   const QString category, const QString &modId, const QVector<PDependency> &dependencies = {},
                   const QString &filename = "", const QString &location = "", const QStringList &iconpaths = {},
                   const QString &oglocation = "", bool isSelected = false);
    bool insertMod(const PMod &mod);
    bool deleteMod(const QString &modId);
    bool updateMod(const QString &modId, const QString &key, const QString &value);
    bool addDependency(const QString &modId, const PDependency &dependency);
    bool removeDependency(const QString &dependencyId);

    QSqlQuery getAllMods();
    QSqlQuery queryMods(const QString &propertyName, const QString &searchTerm);
    QStringList searchMods(const QString &propertyName, const QString &searchTerm);
    Q_INVOKABLE PModItem getModByPk(const QString &modId);
    PModItem populateModItem(QSqlQuery query);
    PModItem queryToModItem(QSqlQuery query);
    PModItem queryToModItem(QString property, QString value);
    QVector<PModItem> queryToModItems(QString property, QString value);

    void loadModsFromZTDs(const QStringList &ztdList);

    bool doesModExist(const QString &modId);
    bool doesDependencyExist(const QString &dependencyId);
    bool doesKeyExist(const QString &modId, const QString &key);

private:
    QSqlDatabase m_db;
    const QString m_dbName = "panda-ldr.padb";
    QString m_dbPath;
    const QString m_tableName = "mods";
    const QString m_createTableQuery = 
        "CREATE TABLE IF NOT EXISTS mods ("
        "pk INTEGER PRIMARY KEY AUTOINCREMENT, "
        "title TEXT NOT NULL, "
        "author TEXT NOT NULL, "
        "description TEXT, "
        "enabled INTEGER NOT NULL, "
        "category TEXT, "
        "tags TEXT, "
        "version TEXT NOT NULL, "
        "mod_id TEXT NOT NULL UNIQUE, "
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
        
    
};

// Declare metatypes
Q_DECLARE_METATYPE(PDatabaseMgr::PDependency)
Q_DECLARE_METATYPE(QVector<PDatabaseMgr::PDependency>)

#endif // PDATABASEMGR_H
