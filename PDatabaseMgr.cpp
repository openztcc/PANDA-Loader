#include "PDatabaseMgr.h"

PDatabaseMgr::PDatabaseMgr() {
    m_dbPath = QCoreApplication::applicationDirPath() + QDir::separator() + m_dbName;
    // remove old connection
    if (QSqlDatabase::contains(m_dbPath)) {
        QSqlDatabase::removeDatabase(m_dbPath);
    }
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(m_dbPath);

    if (!openDatabase()) {
        qDebug() << "Failed to open database";
    }
}

PDatabaseMgr::~PDatabaseMgr() {}

bool PDatabaseMgr::openDatabase() {

    if (!m_db.open()) {
        qDebug() << "Failed to open database: " << m_db.lastError();
        return false;
    }

    // Check if any tables exist
    QSqlQuery query(m_db);
    query.exec("SELECT COUNT(*) FROM sqlite_master WHERE type='table'");

    bool isNewDatabase = true;

    if (query.next()) {
        int tableCount = query.value(0).toInt();
        if (tableCount > 0) {
            isNewDatabase = false;
        }
    } else {
        qDebug() << "Error checking for existing tables:" << query.lastError().text();
        return false;
    }

    if (isNewDatabase) {
        qDebug() << "Database is brand new, initializing tables...";
        return createTables();
    }

    return true;
}

void PDatabaseMgr::closeDatabase() {
    m_db.close();
}

bool PDatabaseMgr::createTables() {
    QSqlQuery query(m_db);
    if (!query.exec(m_createTableQuery)) {
        qDebug() << "Failed to create table: " << query.lastError();
        return false;
    }

    if (!query.exec(m_createDependenciesTableQuery)) {
        qDebug() << "Failed to create dependencies table: " << query.lastError();
        return false;
    }

    return true;
}

bool PDatabaseMgr::insertMod(const QString &name, const QString &desc, const QVector<QString> &authors,
                             const QString &version, const QString &path, bool enabled, const QVector<QString> &tags,
                             const QString &modId, const QVector<PDependency> &dependencies) {
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO mods (title, author, description, path, enabled, tags, version, mod_id) "
                  "VALUES (:title, :author, :description, :path, :enabled, :tags, :version, :mod_id)");
    query.bindValue(":title", name);
    query.bindValue(":author", authors.join(", "));
    query.bindValue(":description", desc);
    query.bindValue(":path", path);
    query.bindValue(":enabled", enabled);
    query.bindValue(":tags", tags.join(", "));
    query.bindValue(":version", version);
    query.bindValue(":mod_id", modId);

    if (!query.exec()) {
        qDebug() << "Failed to insert mod: " << query.lastError();
        return false;
    }

    // Insert dependencies
    if (!addDependency(modId, dependencies)) {
        return false;
    }

    return true;
}

bool PDatabaseMgr::deleteMod(const QString &modId) {
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM mods WHERE mod_id = :mod_id");
    query.bindValue(":mod_id", modId);

    if (!query.exec()) {
        qDebug() << "Failed to delete mod: " << query.lastError();
        return false;
    }

    return true;
}

bool PDatabaseMgr::updateMod(const QString &modId, const QString &key, const QString &value) {
    QSqlQuery query(m_db);
    query.prepare("UPDATE mods SET " + key + " = :value WHERE mod_id = :mod_id");
    query.bindValue(":value", value);
    query.bindValue(":mod_id", modId);

    if (!query.exec()) {
        qDebug() << "Failed to update mod: " << query.lastError();
        return false;
    }

    return true;
}

bool PDatabaseMgr::addDependency(const QString &modId, const PDependency &dependency) {
    QSqlQuery query(m_db);
    // Connect mod_id to dependency_id
    query.prepare("INSERT INTO mods_dependencies (mod_id, dependency_id) "
                  "VALUES (:mod_id, :dependency_id)");

    // Insert main dependency data into dependencies table
    query.prepare("INSERT INTO dependencies (mod_id, name, min_version, optional, ordering, link) "
                  "VALUES (:mod_id, :name, :min_version, :optional, :ordering, :link)");
    query.bindValue(":mod_id", modId);
    query.bindValue(":name", dependency.name);
    query.bindValue(":min_version", dependency.min_version);
    query.bindValue(":optional", dependency.optional);
    query.bindValue(":ordering", dependency.ordering);
    query.bindValue(":link", dependency.link);

    if (!query.exec()) {
        qDebug() << "Failed to add dependency: " << query.lastError();
        return false;
    }

    return true;
}

bool PDatabaseMgr::removeDependency(const QString &modId, const QString &dependencyId) {
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM dependencies WHERE mod_id = :mod_id AND dependency_id = :dependency_id");
    query.bindValue(":mod_id", modId);
    query.bindValue(":dependency_id", dependencyId);

    if (!query.exec()) {
        qDebug() << "Failed to remove dependency: " << query.lastError();
        return false;
    }

    return true;
}
