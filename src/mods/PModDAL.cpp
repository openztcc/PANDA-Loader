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
    QSqlQuery deletedMods = m_db.runOperation(PDatabase::Operation::Delete, table, conditions);
    if (deletedMods.lastError().isValid()) {
        qDebug() << "Error deleting mod: " << deletedMods.lastError().text();
        return false;
    }
}

bool PModDal::updateMod(const QString &table, const QMap<QString, QVariant> &setFields, const QMap<QString, QVariant> &whereConditions) {
    QSqlQuery updatedMods = m_db.runOperation(PDatabase::Operation::Update, table, setFields, {}, "", whereConditions);
    if (updatedMods.lastError().isValid()) {
        qDebug() << "Error updating mod: " << updatedMods.lastError().text();
        return false;
    }
    return true;
}

bool PModDal::doesModExist(const QString &modId) {
    QSqlQuery doesExist = m_db.runOperation(PDatabase::Operation::Select, "mods", {{"mod_id", modId}});
    if (doesExist.lastError().isValid()) {
        qDebug() << "Error checking if mod exists: " << doesExist.lastError().text();
        return false;
    }
    if (doesExist.size() > 0) {
        return true; // Mod exists
    }
}

// TODO: update runOperation to handle select all queries so that user can potentially
// use orderBy and groupBy to get all mods in a certain order or group
QSqlQuery PModDal::getAllMods() {
    return m_db.returnQuery("SELECT * FROM mods ORDER BY title");
}

// Get search results as a list of strings
QVector<QSharedPointer<PModItem>> PModDal::searchMods(PDatabase::Operation operation, const QString &propertyName, const QString &searchTerm) {
    QSqlQuery query = m_db.runOperation(operation, "mods", {propertyName, searchTerm});
    QVector<QSharedPointer<PModItem>> modItems = QVector<QSharedPointer<PModItem>>();

    while (query.next()) {
        QSharedPointer<PModItem> modItem = QSharedPointer<PModItem>::create(query);
        modItems.append(modItem);
    }
    return modItems;
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