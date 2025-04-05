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
                             const QString category, const QString &modId, const QVector<PDependency> &dependencies) 
                             {
    QSqlQuery query(m_db);

    // Check for missing required fields
    if (name.isEmpty() || version.isEmpty() || path.isEmpty() || modId.isEmpty()) {
        qDebug() << "Missing required fields for mod insert";
        return false;
    }

    query.prepare("INSERT INTO mods (title, author, description, path, enabled, tags, category, version, mod_id) "
                  "VALUES (:title, :author, :description, :path, :enabled, :tags, :category, :version, :mod_id)");
    
    // Bind required values
    query.bindValue(":title", name);
    query.bindValue(":version", version);
    query.bindValue(":mod_id", modId);
    query.bindValue(":path", path);
    
    // add authors to author field
    if (!authors.isEmpty()) {
        query.bindValue(":author", authors.join(", "));
    }
    else {
        query.bindValue(":author", "");
    }

    // add description to description field
    if (!desc.isEmpty()) {
        query.bindValue(":description", desc);
    }
    else {
        query.bindValue(":description", "");
    }

    // add path to path field
    if (!path.isEmpty()) {
        query.bindValue(":path", path);
    }
    else {
        query.bindValue(":path", "");
    }

    // add enabled to enabled field
    if (enabled) {
        query.bindValue(":enabled", 1);
    }
    else {
        query.bindValue(":enabled", 0);
    }

    // add tags to tags field
    if (!tags.isEmpty()) {
        query.bindValue(":tags", tags.join(", "));
    }
    else {
        query.bindValue(":tags", "");
    }

    // add category to category field
    if (!tags.isEmpty()) {
        query.bindValue(":category", category);
    }
    else {
        query.bindValue(":category", "Uncategorized");
    }

    // Execute the query
    if (!query.exec()) {
        qDebug() << "Failed to insert mod: " << query.lastError();
        return false;
    }

    // Insert dependencies
    if (!dependencies.isEmpty()) {
        for (const PDependency &dependency : dependencies) {
            if (!addDependency(modId, dependency)) {
                return false;
            }
        }
    }
    else {
        // 
    }

    return true;
}

// TODO: Fix tags so they insert as a list
bool PDatabaseMgr::insertMod(const PMod &mod) {
    return insertMod(mod.title, mod.description, {mod.authors}, mod.version, mod.path, mod.enabled, mod.tags, mod.category, mod.mod_id, mod.dependencies);
}

bool PDatabaseMgr::deleteMod(const QString &modId) {
    QSqlQuery query(m_db);
    
    // Check if mod exists
    query.prepare("SELECT COUNT(*) FROM mods WHERE mod_id = :mod_id");
    query.bindValue(":mod_id", modId);
    if (!query.exec()) {
        qDebug() << "Error running query: " << query.lastError();
        return false;
    }

    // Get the mod_id count
    if (query.next()) {
        int count = query.value(0).toInt();
        if (count == 0) {
            qDebug() << "Mod does not exist";
            return false;
        }
    }
    else {
        qDebug() << "Error getting mod_id count: " << query.lastError();
        return false;
    }

    // Delete the mod
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

    // Check if mod exists
    if (!doesModExist(modId)) {
        return false;
    }

    // Check if key exists
    if (!doesKeyExist(modId, key)) {
        return false;
    }

    // Update the mod
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

    // Check if mod exists
    if (!doesModExist(modId)) {
        return false;
    }
    
    // Insert dependency data into dependencies table
    query.prepare("INSERT INTO dependencies (mod_id, dependency_id, name, min_version, optional, ordering, link) "
                  "VALUES (:mod_id, :dependency_id, :name, :min_version, :optional, :ordering, :link)");
    
    if (dependency.modId.isEmpty() || modId.isEmpty() || dependency.dependencyId.isEmpty()) {
        qDebug() << "Dependency modId is empty";
        return false;
    }
    else {
        query.bindValue(":mod_id", modId);
        query.bindValue(":dependency_id", dependency.modId);
    }

    if (dependency.name.isEmpty()) {
        query.bindValue(":name", "");
    } 
    else {
        query.bindValue(":name", dependency.name);
    }

    if (dependency.min_version.isEmpty()) {
        query.bindValue(":min_version", "");
    } 
    else {
        query.bindValue(":min_version", dependency.min_version);
    }

    if (dependency.optional) {
        query.bindValue(":optional", 1);
    } 
    else {
        query.bindValue(":optional", 0);
    }

    if (dependency.ordering.isEmpty()) {
        query.bindValue(":ordering", "");
    } 
    else {
        query.bindValue(":ordering", dependency.ordering);
    }

    if (dependency.link.isEmpty()) {
        query.bindValue(":link", "");
    } 
    else {
        query.bindValue(":link", dependency.link);
    }

    if (!query.exec()) {
        qDebug() << "Failed to add dependency: " << query.lastError();
        return false;
    }

    return true;
}


//TODO: Fix so it removes dependency from mod, not just dependencies table
bool PDatabaseMgr::removeDependency(const QString &dependencyId) {
    QSqlQuery query(m_db);

    // Remove the dependency from dependencies table
    query.prepare("DELETE FROM dependencies WHERE dependency_id = :dependency_id");
    query.bindValue(":dependency_id", dependencyId);

    if (!query.exec()) {
        qDebug() << "Failed to remove dependency: " << query.lastError();
        return false;
    }

    return true;
}

bool PDatabaseMgr::doesModExist(const QString &modId) {
    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(*) FROM mods WHERE mod_id = :mod_id");
    query.bindValue(":mod_id", modId);

    if (!query.exec()) {
        qDebug() << "Error running query: " << query.lastError();
        return false;
    }

    if (query.next()) {
        int count = query.value(0).toInt();
        if (count == 0) {
            return false;
        }
    }
    else {
        qDebug() << "Error getting mod_id count: " << query.lastError();
        return false;
    }

    return true;
}

bool PDatabaseMgr::doesDependencyExist(const QString &dependencyId) {
    QSqlQuery query(m_db);

    // Check if dependency exists
    query.prepare("SELECT COUNT(*) FROM dependencies WHERE dependency_id = :dependency_id");
    query.bindValue(":dependency_id", dependencyId);

    if (!query.exec()) {
        qDebug() << "Error running query: " << query.lastError();
        return false;
    }

    if (query.next()) {
        int count = query.value(0).toInt();
        if (count == 0) {
            return false;
        }
    }
    else {
        qDebug() << "Error getting dependency count: " << query.lastError();
        return false;
    }

    return true;
}

bool PDatabaseMgr::doesKeyExist(const QString &modId, const QString &key) {
    QSqlQuery query(m_db);
    
    // Check if mod exists
    if (!doesModExist(modId)) {
        return false;
    }

    // Check if key exists
    query.prepare("SELECT COUNT(*) FROM mods WHERE mod_id = :mod_id AND " + key + " IS NOT NULL");
    query.bindValue(":mod_id", modId);

    if (!query.exec()) {
        qDebug() << "Error running query: " << query.lastError();
        return false;
    }

    if (query.next()) {
        int count = query.value(0).toInt();
        if (count == 0) {
            return false;
        }
    }
    else {
        qDebug() << "Error getting key count: " << query.lastError();
        return false;
    }

    return true;
}

QSqlQuery PDatabaseMgr::getAllMods() {
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM mods");
    query.exec();
    return query;
}

// Return results within orderBy filter and searchTerm
// TODO: Handle case where searchTerm is empty or just spaces, should return all mods
// in this filter
QSqlQuery PDatabaseMgr::searchMods(const QString &propertyName, const QString &searchTerm) {
    QSqlQuery query(m_db);

    QString property = propertyName;

    // Default order by title if not specified
    if (property.isEmpty()) {
        property = "title"; 
    }

    qDebug() << "Searching mods with order by: " << property << " and search term: " << searchTerm;

    QString queryStr = QString("SELECT * FROM mods WHERE %1 LIKE :searchTerm ORDER BY title").arg(propertyName);
    query.prepare(queryStr);
    query.bindValue(":searchTerm", "%" + searchTerm + "%");
    
    if (!query.exec()) {
        qDebug() << "Error running search query: " << query.lastError().text();
    }

    return query;
}

// Get search results as a list of strings
QStringList PDatabaseMgr::searchMods(const QString &propertyName, const QString &searchTerm) {
    QSqlQuery query = searchMods(propertyName, searchTerm);
    QStringList results;

    while (query.next()) {
        results.append(query.value("title").toString());
    }

    return results;
}

// Return mod by primary key
PDatabaseMgr::PMod PDatabaseMgr::getModByPk(const QString &modId) {
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM mods WHERE mod_id = :modId");
    query.bindValue(":modId", modId);

    if (!query.exec()) {
        qDebug() << "Error running query: " << query.lastError();
    }

    PMod mod;

    if (query.next()) {
        mod.title = query.value("title").toString();
        mod.authors = query.value("author").toString().split(", ");
        mod.description = query.value("description").toString();
        mod.path = query.value("path").toString();
        mod.enabled = query.value("enabled").toBool();
        mod.tags = query.value("tags").toString().split(", ");
        mod.category = query.value("category").toString();
        mod.version = query.value("version").toString();
        mod.mod_id = query.value("mod_id").toString();
    }

    return mod;
}

// Static version of getModByPk
PDatabaseMgr::PMod PDatabaseMgr::getModByPk(QSqlDatabase &db, const QString &modId) {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM mods WHERE mod_id = :mod_id");
    query.bindValue(":mod_id", modId);

    if (!query.exec()) {
        qDebug() << "Error running query: " << query.lastError();
    }

    PMod mod;

    if (query.next()) {
        mod.title = query.value("title").toString();
        mod.authors = query.value("author").toString().split(", ");
        mod.description = query.value("description").toString();
        mod.path = query.value("path").toString();
        mod.enabled = query.value("enabled").toBool();
        mod.tags = query.value("tags").toString().split(", ");
        mod.category = query.value("category").toString();
        mod.version = query.value("version").toString();
        mod.mod_id = query.value("mod_id").toString();
    }

    return mod;
}