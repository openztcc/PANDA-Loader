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

// Grabs mods from ZTDs and stores them in database
// TODO: Add any errors to a list of errors to display to user
// TODO: Add a check to see if mod already exists in database
// TODO: Add meta.toml file to ztd if it doesn't exist
// TODO: If meta.toml does not exist, add to list of errors for user
// TODO: Let user decide if it's a duplicate or not
void PModDal::loadModsFromFile(const QStringList &ztdList)
{
    // Insert mods into database
    for (const QString &ztd : ztdList)
    {
        // Check if ztd already exists in database
        QVector<QSharedPointer<PModItem>> existingMods = searchMods(PDatabase::Operation::Select, "filename", filename);
        if (!existingMods.isEmpty()) {
            qDebug() << "ZTD already exists in database: " << filename;
            continue; // Skip this mod
        }
        
        // PModItem mod;
        QString filename = ztd.split("/").last();
        QStringList location = ztd.split("/").removeLast().join("/");
        // QStringList iconPaths;
        PConfigMgr meta(nullptr, ztd);

        PFile ztdFile(ztd);
        PFileData metaData = ztdFile.read("meta.toml");
        bool foundMeta = !metaData.isEmpty();

        PConfig meta(nullptr, metaData);
        if (!meta.isValid()) {
            qDebug() << "Meta file is not valid: " << ztd;
            continue; // Skip this mod
        }

        // Initially will only support mods that add content to the game ("hacks" not supported yet)
        QList<PFileData> entryPoints = ztdFile.readAll({"animals/", "scenery/other/"}, {"ucb", "uca", "ucs", "ai"});

        // Stub for later: maybe scan all files in the ztd, then look for entrypoint files. If entrypoint files
        // already exist, then "hack". If not, then "content". Then pop files from list as the entrypoint connects to
        // them. Any remaining files will need to be scanned for duplicates in database. If duplicates exist, then
        // those are "hacks".
        // Or: if no entrypoint files exist, just load file with default data.

        // If no meta file found and 
        int contentModsDetected = entryPoints.size();
        if (contentModsDetected == 0 && !foundMeta) {
            qDebug() << "No content mods detected in ztd: " << ztd;
            mod.setTitle(filename);
            mod.setAuthors({"Unknown"});
            mod.setDescription("No description found");
            mod.setLocation(location);
            mod.setFilename(filename);
            mod.setEnabled(true);
            mod.setCategory("Unknown");
            mod.setTags({"Unknown"});
            mod.setVersion("1.0.0");
            mod.setId(QUuid::createUuid().toString(QUuid::WithoutBraces));
            mod.setIconPaths({});
            mod.setOGLocation(ztd);
            mod.setSelected(false);
            insertMod(mod);
        }


        // Check if config exists
        if (!foundMeta) {
            qDebug() << "No meta config found in ztd: " << ztd;

            // See if we can fill in the blanks
            // TODO: decouple this section into its own entitytype class
            // that reads in the config and determines values

            if (contentModsDetected > 0) {
                for (const PFileData &entryPoint : entryPoints) {

                    // --------------------- Get Type, Category, and determine Icon paths ---------------------
                    // Get the type
                    PConfig config(nullptr, entryPoint);
                    QString type = config.getValue("Type", "Global").toString(); // this is usually the "codename" of the mod
                    if (type.isEmpty()) {
                        // otherwise type is the filename of the entrypoint
                        type = entryPoint.filename.split(".").first();
                    }
                    QMap<QString, QString> iconPathList;
                    QStringList oIcon = config.getValue("Icon", "Icon").toString();
                    QString fIcon;
                    QString mIcon;
                    bool foundIcon = !oIcon.isEmpty() || oIcon == "";

                    // if oIcon is found, then it's an object
                
                    if (!foundIcon) {
                        // try to find unit icons
                        fIcon = config.getValue("f/Icon", "Icon").toString();
                        mIcon = config.getValue("m/Icon", "Icon").toString();
                    }

                    bool foundIcon = !fIcon.isEmpty() || fIcon == "" || !mIcon.isEmpty() || mIcon == "";

                    // if fIcon and/or m/Icon are found, then it's a unit

                    if (foundIcon) {
                        // process the graphics into png files
                        if (!oIcon.isEmpty()) {
                            // process the graphics into png files
                            QStringList pathsToGraphics = PGraphics::generateGraphicsAsPng(oIcon, ztd);
                            mod.setIconPaths(pathsToGraphics);
                        }
                        else if (!fIcon.isEmpty()) {
                            // process the graphics into png files
                            QStringList pathsToGraphics = PGraphics::generateGraphicsAsPng({fIcon, mIcon}, ztd);
                            mod.setIconPaths(pathsToGraphics);
                        }
                    }

                    // ---------------------- Get Description, Title -----------------------
                    QString description = config.getValue("cLongHelp", "1033").toString();
                    QString title = config.getValue("cName", "1033").toString();
                    QString category = determineCategory(config);
                    if (category.isEmpty()) {
                        category = "Unknown";
                    }
                    QStringList tags = generateTagsFromConfig(config);

                    // ---------------------- File meta data -----------------------
                    QString fileSize = QFile(ztd).size();
                    QString fileDate = QFileInfo(ztd).lastModified().toString("yyyy-MM-dd hh:mm:ss");
                    QString fileName = QFileInfo(ztd).fileName();
                    QString filePath = QFileInfo(ztd).absoluteFilePath();
                    QString fileLocation = QFileInfo(ztd).absolutePath();

                    // ---------------------- Remaining ------------------------
                    QString 
                }
            }
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

QString PModDal::determineCatFromExt(const PFileData &fileData) {
    switch (fileData.ext) {
        case "ucb":
            return "Building";
        case "ucs":
            return "Scenery";
        case "uca":
            return "Animals";
        case "ai":
            QString pathParts = fileData.path.split("/");
            // category is always the first part of the path
            QString category = pathParts[0];
            // return proper case for category
            return category.toUpper().left(1) + category.mid(1).toLower();
        default:
            return "Unknown";
    }
}

QMap<QString, QString> PModDal::