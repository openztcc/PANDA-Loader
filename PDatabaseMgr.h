#ifndef PDATABASEMGR_H
#define PDATABASEMGR_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>

class PDatabaseMgr : public QObject
{
    Q_OBJECT
public:
    PDatabaseMgr();
    ~PDatabaseMgr();

    struct PDependency
    {
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
    bool insertMod(const QString &name, const QString &desc, const std::vector<QString> &authors, 
                   const QString &version, const QString &path, bool enabled, const std::vector<QString> &tags,
                   const QString &modId, std::vector<PDependency> &dependencies);
    bool deleteMod(const QString &modId);
    bool updateMod(const QString &modId, const QString &key, const QString &value);
    bool addDependency(const QString &modId, const PDependency &dependency);
    bool removeDependency(const QString &modId, const QString &dependencyId);

    QSqlQuery getAllMods();
    QSqlQuery orderBy(const QString &query);
    QSqlQuery searchMods(const QString &searchTerm);
    QSqlQuery getModByPk(const QString &modId);


private:
    QSqlDatabase m_db;
    const QString m_dbName = "PandaLdr.db";
    const QString m_tableName = "mods";
    const QString m_createTableQuery = 
        "CREATE TABLE IF NOT EXISTS mods ("
        "pk INTEGER PRIMARY KEY AUTOINCREMENT, "
        "title TEXT NOT NULL, "
        "author TEXT NOT NULL, "
        "description TEXT, "
        "path TEXT NOT NULL, "
        "enabled INTEGER NOT NULL, "
        "category TEXT, "
        "tags TEXT, "
        "version TEXT NOT NULL, "
        "mod_id TEXT NOT NULL UNIQUE"
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

#endif // PDATABASEMGR_H
