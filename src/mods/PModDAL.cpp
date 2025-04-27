#include "PModDal.h"

PModDal::PModDal() {
    m_db = PDatabase(QDir::homePath() + "/panda.padb", "ModDal");
    if (!m_db.open()) {
        qDebug() << "Failed to open database in PModDal";
    }
}

bool PModDal::insertMod(const PModItem &mod) 
{
    QVariantMap params;
    params.insert(":title", mod.title());
    params.insert(":authors", mod.authors().join(", "));
    params.insert(":description", mod.description());
    params.insert(":enabled", mod.enabled() ? 1 : 0);
    params.insert(":category", mod.category());
    params.insert(":tags", mod.tags().join(", "));
    params.insert(":version", mod.version());
    params.insert(":mod_id", mod.id());
    params.insert(":dep_id", mod.dependencyId());
    params.insert(":filename", mod.filename());
    params.insert(":location", mod.location());
    params.insert(":iconpaths", mod.iconpaths().join(", "));
    params.insert(":oglocation", mod.oglocation());
    params.insert(":is_selected", mod.selected() ? 1 : 0);
    
    return m_db.runQuery(m_insertModQuery, params);
}

bool PModDal::deleteMod(const QString &table, const QMap<QString, QVariant> &conditions) {
    m_db.runOperation(PDatabase::Operation::Delete, table, conditions);
}

bool PModDal::updateMod(const QString &table, const QMap<QString, QVariant> &setFields, const QMap<QString, QVariant> &whereConditions) {
    m_db.runOperation(PDatabase::Operation::Update, table, setFields, {}, whereConditions);
}

bool PModDal::doesModExist(const QString &modId) {
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

QSqlQuery PModDal::getAllMods() {
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM mods");
    query.exec();
    return query;
}

// Return results within orderBy filter and searchTerm
// TODO: Handle case where searchTerm is empty or just spaces, should return all mods
// in this filter
QSqlQuery PModDal::queryMods(const QString &propertyName, const QString &searchTerm) {
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
QStringList PModDal::searchMods(const QString &propertyName, const QString &searchTerm) {
    QSqlQuery query = queryMods(propertyName, searchTerm);
    QStringList results;

    while (query.next()) {
        results.append(query.value("title").toString());
    }

    return results;
}

// Static version of getModByPk
QSharedPointer<PModItem> PModDal::getModByPk(const QString &modId) {
    return queryToModItem("mod_id", modId);
}

// Grabs mods from ZTDs and stores them in database
// TODO: Add any errors to a list of errors to display to user
// TODO: Add a check to see if mod already exists in database
// TODO: Add meta.toml file to ztd if it doesn't exist
// TODO: If meta.toml does not exist, add to list of errors for user
// TODO: Let user decide if it's a duplicate or not
void PModDal::loadModsFromZTDs(const QStringList &ztdList)
{
    // open database
    // PModDal db;
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
QSharedPointer<PModItem> PModDal::populateModItem(QSqlQuery &query) {
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
QSharedPointer<PModItem> PModDal::queryToModItem(QSqlQuery &query) {
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
QSharedPointer<PModItem> PModDal::queryToModItem(QString property, QString value) {
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM mods WHERE " + property + " = :value");
    query.bindValue(":value", value);

    return queryToModItem(query);
}

// Get a query result as a list of PModItem objects
QVector<QSharedPointer<PModItem>> PModDal::queryToModItems(QString property, QString value) {
    QSqlQuery query = queryMods(property, value);
    QVector<QSharedPointer<PModItem>> modItems;

    while (query.next()) {
        QSharedPointer<PModItem> modItem = QSharedPointer<PModItem>::create();
        modItem = populateModItem(query);
        modItems.append(modItem);
    }

    return modItems;
}
