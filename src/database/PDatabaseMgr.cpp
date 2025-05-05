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

bool PDatabaseMgr::insertMod(const QString &title, const QString &description, const QStringList &authors, 
    const QString &version, bool enabled, const QStringList &tags, const QString &category, 
    const QString &id, const QString &depId, const QString &filename,
    const QString &location, const QStringList &iconpaths, const QString &oglocation,
    bool selected, QObject *parent)
                             {
    QSqlQuery query(m_db);

    // Check for missing required fields
    if (title.isEmpty() || version.isEmpty() || id.isEmpty()) {
        qDebug() << "Missing required fields for mod insert";
        return false;
    }

    query.prepare("INSERT INTO mods (title, authors, description, enabled, category, tags, version, mod_id, dep_id, filename, location, iconpaths, oglocation, is_selected) "
                  "VALUES (:title, :authors, :description, :enabled, :category, :tags, :version, :mod_id, :dep_id, :filename, :location, :iconpaths, :oglocation, :is_selected)");
    
    // Bind required values
    query.bindValue(":title", title);
    query.bindValue(":version", version);
    query.bindValue(":mod_id", id);
    
    // add authors to author field
    if (!authors.isEmpty()) {
        query.bindValue(":authors", authors.join(", "));
    }
    else {
        query.bindValue(":authors", "");
    }

    // add description to description field
    if (!description.isEmpty()) {
        query.bindValue(":description", description);
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

    // Insert dep id
    if (!depId.isEmpty()) {
        query.bindValue(":dep_id", depId);
    } 
    else {
        query.bindValue(":dep_id", "");
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

    if (selected) {
        query.bindValue(":is_selected", 1);
    } 
    else {
        query.bindValue(":is_selected", 0);
    }

    // Execute the query
    if (!query.exec()) {
        qDebug() << "Failed to insert mod: " << query.lastError();
        return false;
    }

    return true;
}

// TODO: Fix tags so they insert as a list
bool PDatabaseMgr::insertMod(const PModItem &mod) {
    return insertMod(mod.title(), mod.description(), {mod.authors()}, mod.version(), mod.enabled(), mod.tags(), mod.category(), mod.id(), mod.dependencyId(),
        mod.filename(), mod.location(), mod.iconpaths(), mod.oglocation(), mod.selected());
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
QSharedPointer<PModItem> PDatabaseMgr::getModByPk(const QString &modId) {
    return queryToModItem("mod_id", modId);
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
        PModItem mod;
        QString filename = ztd.split("/").last();
        QStringList locationPath = ztd.split("/");
        locationPath.removeLast();
        QString location = locationPath.join("/");
        QStringList iconPaths;
        PConfigMgr meta(nullptr, ztd);


        // Check if ztd already exists in database
        if (searchMods("filename", filename).size() > 0) {
            qDebug() << "ZTD already exists in database: " << filename;
            continue;
        } else { // process icons if do not exist
            QMap<QString, OutputBuffer> buffers = PApeFile::getGraphicBuffers(ztd);
            if (buffers.isEmpty()) {
                qDebug() << "No buffers to process for ztd: " << ztd;
            } else {
                iconPaths = PApeFile::processIcons(buffers);
            }
        }

        // Check if config exists
        if (!PZtdMgr::fileExistsInZtd(ztd, "meta.toml")) {
            qDebug() << "No meta config found in ztd: " << ztd;

            // Insert mod with blank values
            mod.setTitle("Unknown");
            mod.setAuthors({"Unknown"});
            mod.setDescription("No description found");
            mod.setLocation(location);
            mod.setFilename(filename);
            mod.setEnabled(true);
            mod.setCategory("Unknown");
            mod.setTags({"Unknown"});
            mod.setVersion("1.0.0");
            mod.setId(QUuid::createUuid().toString(QUuid::WithoutBraces));
            mod.setIconPaths(iconPaths);
            mod.setOGLocation(ztd);
            mod.setSelected(false);
        }
        else {

            // Get meta config from ztd
            QByteArray fileData = PZtdMgr::getFileFromRelPath(ztd, "meta.toml");
            std::istringstream tomlStream(fileData.toStdString());
            toml::table config = toml::parse(tomlStream);

            // Grab mod_id from config
            mod.setId(meta.getValue("", "mod_id").toString());
            if (mod.id().isEmpty()) {
                mod.setId(QUuid::createUuid().toString(QUuid::WithoutBraces));
            }

            // Check if mod_id already exists in database
            // TODO: Let user decide if it's a duplicate or not
            if (doesModExist(mod.id())) {
                qDebug() << "Mod already exists in database: " << mod.id();
                continue; // Skip this mod
            }

            // Get other values from config
            mod.setTitle(meta.getValue("", "name").toString());
            if (mod.title().isEmpty()) {
                mod.setTitle("Unknown");
            }

            mod.setAuthors(meta.getValue("", "authors").toStringList());
            if (mod.authors().isEmpty()) {
                mod.setAuthors({"Unknown"});
            }

            mod.setDescription(meta.getValue("", "description").toString());
            if (mod.description().isEmpty()) {
                mod.setDescription("No description found");
            }

            mod.setEnabled(true);

            mod.setTags(meta.getValue("", "tags").toStringList());
            // remove "All" from tags if it exists
            QStringList tags = mod.tags();
            tags.removeAll("All");
            mod.setTags(tags);
            if (mod.tags().isEmpty()) {
                mod.setTags({"Unknown"});
            }

            mod.setCategory(mod.tags()[0]);
            qDebug() << "Added category: " << mod.category() << " to mod " << mod.title();
            if (mod.category().isEmpty()) {
                mod.setCategory("Unknown");
            }

            mod.setVersion(meta.getValue("", "version").toString());
            if (mod.version().isEmpty()) {
                mod.setVersion("1.0.0");
            }

            mod.setFilename(filename);
            mod.setLocation(location);
            mod.setIconPaths(iconPaths);
            mod.setOGLocation(location);
            mod.setSelected(false);
        }

        insertMod(mod);

    }

    // // close database
    // db.closeDatabase();
    qDebug() << "Loaded mods from ZTDs";
}

// Populate mod item from query result
QSharedPointer<PModItem> PDatabaseMgr::populateModItem(QSqlQuery &query) {
    QSharedPointer<PModItem> modItem = QSharedPointer<PModItem>::create();

    modItem->setTitle(query.value("title").toString());
    modItem->setAuthors(query.value("authors").toString().split(", "));
    modItem->setDescription(query.value("description").toString());
    modItem->setEnabled(query.value("enabled").toBool());
    modItem->setCategory(query.value("category").toString());
    modItem->setTags(query.value("tags").toString().split(", "));
    modItem->setId(query.value("mod_id").toString());
    modItem->setFilename(query.value("filename").toString());
    modItem->setIconPaths(query.value("iconpaths").toString().split(", "));
    modItem->setDependencyId(query.value("dep_id").toString());
    modItem->setLocation(query.value("location").toString());
    modItem->setOGLocation(query.value("oglocation").toString());
    modItem->setSelected(query.value("is_selected").toBool());
    modItem->setVersion(query.value("version").toString());

    return modItem;
}

// Get the first result as a PModItem object
QSharedPointer<PModItem> PDatabaseMgr::queryToModItem(QSqlQuery &query) {
    QSharedPointer<PModItem> modItem = QSharedPointer<PModItem>::create();

    if (!query.exec()) {
        qDebug() << "Error running query:" << query.lastError();
        return modItem;
    }

    if (query.next()) {
        modItem = populateModItem(query);
    } else {
        qDebug() << "Mod not found with ID:" << query.lastError();
        return modItem;
    }

    return modItem;
}

// Get a query result as a PModItem object
QSharedPointer<PModItem> PDatabaseMgr::queryToModItem(QString property, QString value) {
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM mods WHERE " + property + " = :value");
    query.bindValue(":value", value);

    return queryToModItem(query);
}

// Get a query result as a list of PModItem objects
QVector<QSharedPointer<PModItem>> PDatabaseMgr::queryToModItems(QString property, QString value) {
    QSqlQuery query = queryMods(property, value);
    QVector<QSharedPointer<PModItem>> modItems;

    while (query.next()) {
        QSharedPointer<PModItem> modItem = QSharedPointer<PModItem>::create();
        modItem = populateModItem(query);
        modItems.append(modItem);
    }

    return modItems;
}
