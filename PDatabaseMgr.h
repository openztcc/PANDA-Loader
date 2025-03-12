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

class PDatabaseMgr : public QObject
{
    Q_OBJECT
public:
    PDatabaseMgr();
    ~PDatabaseMgr();

    struct PMod
    {
        QString title;
        QString author;
        QString description;
        QString path;
        bool enabled;
        QString category;
        QStringList tags;
        QString version;
        QString mod_id;
        QVector<PDependency> dependencies;
    };

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
                   const QString &version, const QString &path, bool enabled, const QVector<QString> &tags,
                   const QString &modId, const QVector<PDependency> &dependencies);
    bool insertMod(const PMod &mod);
    bool deleteMod(const QString &modId);
    bool updateMod(const QString &modId, const QString &key, const QString &value);
    bool addDependency(const QString &modId, const PDependency &dependency);
    bool removeDependency(const QString &modId, const QString &dependencyId);
    QVector<PMod> getModsByID(const QString &modId);

    QSqlQuery getAllMods();
    QSqlQuery orderBy(const QString &query);
    QSqlQuery searchMods(const QString &searchTerm);
    QSqlQuery getModByPk(const QString &modId);

    bool doesModExist(const QString &modId);
    bool doesDependencyExist(const QString &modId, const QString &dependencyId);
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

// Declare metatypes
Q_DECLARE_METATYPE(PDatabaseMgr::PDependency)
Q_DECLARE_METATYPE(QVector<PDatabaseMgr::PDependency>)

#endif // PDATABASEMGR_H
