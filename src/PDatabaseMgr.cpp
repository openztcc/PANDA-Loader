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
                             const QString &version, bool enabled, const QVector<QString> &tags,
                             const QString category, const QString &modId, const QVector<PDependency> &dependencies,
                             const QString &filename, const QString &location, const QStringList &iconpaths,
                             const QString &oglocation, bool isSelected)
                             {
    QSqlQuery query(m_db);

    // Check for missing required fields
    if (name.isEmpty() || version.isEmpty() || modId.isEmpty()) {
        qDebug() << "Missing required fields for mod insert";
        return false;
    }

    query.prepare("INSERT INTO mods (title, author, description, enabled, tags, category, version, mod_id, filename, location, iconpaths,
                    oglocation, isSelected) "
                  "VALUES (:title, :author, :description, :enabled, :tags, :category, :version, :mod_id, :filename, :location, :iconpaths,
                    :oglocation, :isSelected)");
    
    // Bind required values
    query.bindValue(":title", name);
    query.bindValue(":version", version);
    query.bindValue(":mod_id", modId);
    
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

    // Insert mod location and filename
    if (!location.isEmpty()) {
        query.bindValue(":location", location);
    } 
    else {
        query.bindValue(":location", "");
    }
    if (!filename.isEmpty()) {
        query.bindValue(":filename", filename);
    } 
    else {
        query.bindValue(":filename", "");
    }

    if (!iconpaths.isEmpty()) {
        query.bindValue(":iconpaths", iconpaths.join(", "));
    } 
    else {
        query.bindValue(":iconpaths", "");
    }

    if (!oglocation.isEmpty()) {
        query.bindValue(":oglocation", oglocation);
    } 
    else {
        query.bindValue(":oglocation", "");
    }

    if (isSelected) {
        query.bindValue(":isSelected", 1);
    } 
    else {
        query.bindValue(":isSelected", 0);
    }

    // Execute the query
    if (!query.exec()) {
        qDebug() << "Failed to insert mod: " << query.lastError();
        return false;
    }

    return true;
}

// TODO: Fix tags so they insert as a list
bool PDatabaseMgr::insertMod(const PMod &mod) {
    return insertMod(mod.title, mod.description, {mod.authors}, mod.version, mod.enabled, mod.tags, mod.category, mod.mod_id, mod.dependencies,
        mod.filename, mod.location, mod.iconpaths, mod.oglocation, mod.isSelected);
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
QSqlQuery PDatabaseMgr::queryMods(const QString &propertyName, const QString &searchTerm) {
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
    QSqlQuery query = queryMods(propertyName, searchTerm);
    QStringList results;

    while (query.next()) {
        results.append(query.value("title").toString());
    }

    return results;
}

// Static version of getModByPk
PModItem PDatabaseMgr::getModByPk(const QString &modId) {
    return queryAsObject("mod_id", modId);
}

// Grabs mods from ZTDs and stores them in database
// TODO: Add any errors to a list of errors to display to user
// TODO: Add a check to see if mod already exists in database
// TODO: Add meta.toml file to ztd if it doesn't exist
// TODO: If meta.toml does not exist, add to list of errors for user
// TODO: Let user decide if it's a duplicate or not
void PDatabaseMgr::loadModsFromZTDs(const QStringList &ztdList)
{
    // open database
    // PDatabaseMgr db;
    // if (!db.openDatabase()) {
    //     qDebug() << "Failed to open database for loading mods from ZTDs";
    //     return; // Failed to open database
    // }

    // Insert mods into database
    for (const QString &ztd : ztdList)
    {
        PDatabaseMgr::PMod mod;
        QString filename = ztd.split("/").last();
        QStringList locationPath = ztd.split("/");
        locationPath.removeLast();
        QString location = locationPath.join("/");
        QStringList iconPaths;


        // Check if ztd already exists in database
        if (searchMods("filename", filename).size() > 0) {
            qDebug() << "ZTD already exists in database: " << filename;
            continue;
        } else { // process icons if do not exist
            QMap<QString, OutputBuffer> buffers = PGraphicsMgr::getGraphicBuffers(ztd);
            if (buffers.isEmpty()) {
                qDebug() << "No buffers to process for ztd: " << ztd;
            } else {
                iconPaths = PGraphicsMgr::processIcons(buffers);
            }
        }

        // Check if config exists
        if (!PZtdMgr::fileExistsInZtd(ztd, "meta.toml")) {
            qDebug() << "No meta config found in ztd: " << ztd;

            // Insert mod with blank values
            mod.title = "Unknown";
            mod.authors = {"Unknown"};
            mod.description = "No description found";
            mod.location = location;
            mod.filename = filename;
            mod.enabled = true;
            mod.category = "Unknown";
            mod.tags = {"Unknown"};
            mod.version = "1.0.0";
            mod.mod_id = QUuid::createUuid().toString(QUuid::WithoutBraces);
            mod.iconpaths = iconPaths;
            mod.oglocation = ztd;
            mod.isSelected = false;
        }
        else {

            // Get meta config from ztd
            toml::table config = PConfigMgr::getMetaConfig(ztd);

            // Grab mod_id from config
            mod.mod_id = PConfigMgr::getKeyValue("mod_id", config);
            if (mod.mod_id.isEmpty()) {
                mod.mod_id = QUuid::createUuid().toString(QUuid::WithoutBraces);
            }

            // Check if mod_id already exists in database
            // TODO: Let user decide if it's a duplicate or not
            if (doesModExist(mod.mod_id)) {
                qDebug() << "Mod already exists in database: " << mod.mod_id;
                continue; // Skip this mod
            }

            // Get other values from config
            mod.title = PConfigMgr::getKeyValue("name", config);
            if (mod.title.isEmpty()) {
                mod.title = "Unknown";
            }

            mod.authors = PConfigMgr::getKeyValueAsList("authors", config);
            if (mod.authors.isEmpty()) {
                mod.authors = {"Unknown"};
            }

            mod.description = PConfigMgr::getKeyValue("description", config);
            if (mod.description.isEmpty()) {
                mod.description = "No description found";
            }

            mod.enabled = true;

            mod.tags = PConfigMgr::getKeyValueAsList("tags", config);
            // remove "All" from tags if it exists
            mod.tags.removeAll("All");
            if (mod.tags.isEmpty()) {
                mod.tags = {"Unknown"};
            }

            mod.category = mod.tags[0];
            qDebug() << "Added category: " << mod.category << " to mod " << mod.title;
            if (mod.category.isEmpty()) {
                mod.category = "Unknown";
            }

            if (mod.category.isEmpty()) {
                mod.category = "Unknown";
            }

            mod.version = PConfigMgr::getKeyValue("version", config);
            if (mod.version.isEmpty()) {
                mod.version = "1.0.0";
            }

            mod.filename = filename;
            mod.location = location;
            mod.iconpaths = iconPaths;
            mod.oglocation = ztd;
            mod.isSelected = false;
        }

        insertMod(mod);

    }

    // // close database
    // db.closeDatabase();
    qDebug() << "Loaded mods from ZTDs";
}

// Get a query result as a PModItem object
PModItem PDatabaseMgr::queryToObject(QString property, QString value) {
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM mods WHERE " + property + " = :value");
    query.bindValue(":value", value);

    PModItem modItem;

    if (!query.exec()) {
        qDebug() << "Error running query:" << query.lastError();
        return modItem;
    }

    if (query.next()) {
        modItem.setmodTitle(query.value("title").toString());
        modItem.setmodAuthor(query.value("author").toString());
        modItem.setmodDescription(query.value("description").toString());
        modItem.setmodEnabled(query.value("enabled").toBool());
        modItem.setmodCategory(query.value("category").toString());
        modItem.setmodTags(query.value("tags").toString());
        modItem.setmodId(query.value("mod_id").toString());
        modItem.setmodFilename(query.value("filename").toString());
        modItem.setmodIconPaths(query.value("iconpaths").toString().split(", ", Qt::SkipEmptyParts));
        modItem.setDependencyId(query.value("dependency_id").toString());
        modItem.setmodLocation(query.value("location").toString());
    } else {
        qDebug() << "Mod not found with ID:" << value;
        return modItem;
    }

    return modItem;
}

// Get a query result as a list of PModItem objects
QVector<PModItem> PDatabaseMgr::queryToObjects(QString property, QString value) {
    QSqlQuery query = queryMods(property, value);
    QVector<PModItem> modItems;

    while (query.next()) {
        PModItem modItem;
        modItem.setmodTitle(query.value("title").toString());
        modItem.setmodAuthor(query.value("author").toString());
        modItem.setmodDescription(query.value("description").toString());
        modItem.setmodEnabled(query.value("enabled").toBool());
        modItem.setmodCategory(query.value("category").toString());
        modItem.setmodTags(query.value("tags").toString());
        modItem.setmodId(query.value("mod_id").toString());
        modItem.setmodFilename(query.value("filename").toString());
        modItem.setmodIconPaths(query.value("iconpaths").toString().split(", ", Qt::SkipEmptyParts));
        modItem.setDependencyId(query.value("dependency_id").toString());
        modItem.setmodLocation(query.value("location").toString());

        modItems.append(modItem);
    }

    return modItems;
}