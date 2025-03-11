#include "PDatabaseMgr.h"

PDatabaseMgr::PDatabaseMgr() {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(m_dbName);

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

bool PDatabaseMgr::insertMod(const QString &name, const QString &desc, const std::vector<QString> &authors, 
                             const QString &version, const QString &path, bool enabled, const std::vector<QString> &tags,
                             const QString &modId, std::vector<PDependency> &dependencies) {
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
    for (const auto &dep : dependencies) {
        query.prepare("INSERT INTO dependencies (mod_id, name, min_version, optional, ordering, link) "
                      "VALUES (:mod_id, :name, :min_version, :optional, :ordering, :link)");
        query.bindValue(":mod_id", modId);
        query.bindValue(":name", dep.name);
        query.bindValue(":min_version", dep.min_version);
        query.bindValue(":optional", dep.optional);
        query.bindValue(":ordering", dep.ordering);
        query.bindValue(":link", dep.link);

        if (!query.exec()) {
            qDebug() << "Failed to insert dependency: " << query.lastError();
            return false;
        }
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