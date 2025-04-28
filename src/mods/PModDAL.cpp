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

        // If no meta file found and no entrypoints found, then this might be a misc category
        // of mod (i.e. config tweaks, or anything else that doesn't add content to the game)
        int contentModsDetected = entryPoints.size();

        // if found meta file and more than 1 entrypoint, then this is 
        // a content pack. we will call it a collection.
        // for these kinds of mods, the 'collection' will be the name of the mod
        // and every 'mod' within the collection will be delisted in the modlist.
        // instead, they will be show in the infopane.
        PModItem mod;

        bool MetaCollection = foundMeta && contentModsDetected > 1;
        bool MetaSingle = foundMeta && contentModsDetected == 1;
        bool MetaMisc = foundMeta && contentModsDetected == 0;
        bool NoMetaNoEntry = !foundMeta && contentModsDetected == 0;

        // ------------------------------------------------------------------- Build the base information for the mod

        if (MetaCollection) { // meta.toml found + collection of mods
            // Load the meta file and get the mod data
            toml::table config = toml::parse(metaData.data());
            mod = buildModFromToml(config, ztd);    
        } else if (MetaSingle || MetaMisc) { // meta.toml found + single mod
            // if found meta file and only 1 entrypoint, then this is a single mod. easy!
            toml::table config = toml::parse(metaData.data());
            mod = buildModFromToml(config, ztd);            
        } else if (NoMetaNoEntry) { // no meta file + no entrypoints. user will need to manually configure this mod.
            // if no meta file and no entrypoints, then this is a misc mod
            mod = buildDefaultMod(ztd, entryPoints);            
        } else {
            mod = buildModFromEntryPoints(entryPoints, ztd);
        }

        // ------------------------------------------------------------------- Insert file data (size, date, etc.)
        QMap<QString, QVariant> fileData = generateFileData(ztd);
        mod.setFileDate(fileData["date"].toString());
        mod.setFileSize(fileData["size"].toString());
        mod.setOriginalLocation(fileData["location"].toString());
        mod.setCurrentLocation(fileData["location"].toString());
        mod.setFileName(fileData["filename"].toString());

        // ------------------------------------------------------------------- Set the category for the mod
        if (contentModsDetected > 0) {
            QString category = determineCategory(entryPoints);
            mod.setCategory(category);
        } else {
            mod.setCategory("Not Assigned");
        }

        // ------------------------------------------------------------------- Set the tags for the mod
        if (contentModsDetected > 0) {
            QStringList tags = generateTagsFromConfig(meta);
            mod.setTags(tags);
        } else {
            mod.setTags(QStringList() << "Misc" << "Not Assigned");
        }


        // Check if config exists
        if (!foundMeta) {
            qDebug() << "No meta config found in ztd: " << ztd;

            // See if we can fill in the blanks
            // TODO: decouple this section into its own entitytype class
            // that reads in the config and determines values

            if (contentModsDetected > 0) {
                // The ani paths are the configuration files that contain the paths to the graphics
                QMap<QStringList, QString> iconPaths;
                for (const PFileData &entryPoint : entryPoints) {

                    // --------------------- Get Type, Category, and determine Icon paths ---------------------
                    // Get the type
                    PConfig config(nullptr, entryPoint);
                    QString type = config.getValue("Type", "Global").toString(); // this is usually the "codename" of the mod
                    if (type.isEmpty()) {
                        // otherwise type is the filename of the entrypoint
                        type = entryPoint.filename.split(".").first();
                    }
                    // Get the category
                    QString category = determineCategory(entryPoint);

                    // Get icons and their generated paths. Graphics will be saved in the following format:
                    // type + "_" + <number> + "_" + aniFileName + ".png"
                    // ex: "gsblstnd_1_SE.png"
                    if (category == "Building" || category == "Scenery") {
                        // get icon path for objects
                        QString aniPath = config.getValue("Icon", "Icon").toString();
                        if (!aniPath.isEmpty()) {
                            qDebug() << "ERROR: ani path is empty for: " << entryPoint.path;
                        } else {
                            QString graphicPath = PGraphics::generateGraphicsAsPng(iconPath, ztd);

                            PFile aniFile(ztd);
                            PFileData aniData = aniFile.read(aniPath);
                            PConfig aniConfig(nullptr, aniData);
                            QStringList iconPaths = PGraphics::generateGraphicsAsPng(aniPaths, type);
                            iconPaths.insert(iconPaths, aniPath);
                        }
                    }
                    else if (category == "Animals") {
                        // get icon path for animals
                        QString aniPathF = config.getValue("f/Icon", "Icon").toString();
                        QString aniPathM = config.getValue("m/Icon", "Icon").toString();
                        if (!aniPathF.isEmpty()) {
                            qDebug() << "ERROR: ani path is empty for: " << entryPoint.path;
                        } else {
                            QString graphicPathF = PGraphics::generateGraphicsAsPng(iconPath, ztd);
                            iconPaths.insert(iconPath, graphicPathF);
                        }
                        if (!aniPathM.isEmpty()) {
                            qDebug() << "ERROR: ani path is empty for: " << entryPoint.path;
                        } else {
                            QString graphicPathM = PGraphics::generateGraphicsAsPng(iconPath, ztd);
                            iconPaths.insert(iconPath, graphicPathM);
                        }
                    } 

                    // ---------------------- Get Description, Title -----------------------

                    if (category == "Animals") {}
                        QString description = config.getValue("cLongHelp", "1033").toString();
                        QString title = config.getValue("cName", "1033").toString();
                        QString category = determineCategory(config);
                        if (category.isEmpty()) {
                            category = "Unknown";
                        }
                        QStringList tags = generateTagsFromConfig(config);
                    } else {
                        // TODO: For misc mods that have a cName, we can get the name and description
                        // from the binary language file. Will need to find a library to parse the binary.
                        QString description = "";
                        QString title = ""
                        QString category = determineCategory(config);
                        if (category.isEmpty()) {
                            category = "Unknown";
                        }
                    }

                    // ---------------------- File meta data -----------------------
                    QString fileSize = QFile(ztd).size();
                    QString fileDate = QFileInfo(ztd).lastModified().toString("yyyy-MM-dd hh:mm:ss");
                    QString fileName = QFileInfo(ztd).fileName();
                    QString filePath = QFileInfo(ztd).absoluteFilePath();
                    QString fileLocation = QFileInfo(ztd).absolutePath();

                    // --------------------- Create mod item ---------------------
                    PModItem mod;
                    mod.setId(QUuid::createUuid().toString(QUuid::WithoutBraces));
                    mod.setTitle(title);
                    mod.setAuthors({"Unknown"});
                    mod.setDescription(description);
                    mod.setEnabled(true);
                    mod.setCategory(category);
                    mod.setTags(tags);
                    mod.setVersion("1.0.0");
                    mod.setFilename(fileName);
                    mod.setCollection(fileName);
                    mod.setLocation(fileLocation);
                    mod.setIconPaths(iconPaths);
                    mod.setOGLocation(filePath);
                    mod.setSelected(false);
                    mod.setDependencyId(QUuid::createUuid().toString(QUuid::WithoutBraces));
                    mod.setFileSize(fileSize);
                    mod.setFileDate(fileDate);
                    mod.setFilePath(filePath);
                    mod.setFileLocation(fileLocation);
                    mod.setFileName(fileName);

                    insertMod(mod);
                }
            }
        }
    }

    // // close database
    // db.closeDatabase();
    qDebug() << "Loaded mods from ZTDs";
}

// Determine the category of the mod based on the file extension and path
// Possible categories are: Building, Scenery, Animals, misc, Unknown
QString PModDal::determineCategory(const PFileData &fileData) {
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

// Determine the category of the mod based on the meta.toml file in the root of the ztd
PModItem PModDal::buildModFromToml(const toml::table &config, const QString &ztdPath) {
    PModItem mod;
    QFileInfo fileInfo(ztdPath);
    QString filename = fileInfo.fileName();
    QString location = fileInfo.absolutePath();
    QString fileSize  = QString::number(fileInfo.size());
    QString fileDate = fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");

    // values from TOML file
    mod.setId(config.getValue("mod_id").value_or("Unknown"));
    mod.setTitle(config.getValue("name").value_or("Unknown"));
    mod.setAuthors(config.getValue("authors").value_or({"Unknown"}));
    mod.setDescription(config.getValue("description").value_or("No description found"));
    mod.setVersion(config.getValue("version").value_or("1.0.0"));
    mod.setLink(config.getValue("link").value_or("Unknown"));

    // if dependency table exists, then add the dependency id to the mod and add to db
    QMap<QString, QVariant> depTable = config.getValue("dependencies").value_or({});
    if (!depTable.isEmpty()) {
        PDepDal depDal;
        for (const auto &dep : depTable) {
            PDependency dependency = depDal.addDependency(mod.id(), dep);
            if (dependency.isValid()) {
                mod.setDependencyId(dependency.id());
            }
        }
    }
    // if no dependency table, then set the dependency id to None
    else {
        mod.setDependencyId("None");
    }

    mod.setEnabled(true);
    mod.setListed(true);

    mod.setCategory(""); // this will be determined later from rel path
    mod.setTags(config.getValue("tags").value_or({"Unknown"}));
    mod.setFilename(config.getValue("filename").value_or("Unknown"));
    mod.setFileSize(fileSize);
    mod.setFileDate(fileDate);

    mod.setIconPaths({}); // this will be determined later

    mod.setCurrentLocation(ztdPath); // renamed from setOGLocation
    mod.setDisabledLocation(ztdPath); // new field for disabled location
    mod.setOriginalLocation(ztdPath); // new field for original location

    mod.setSelected(false);

    mod.setDependencyId(config.getValue("dep_id").value_or("None"));

    return mod;
}

// Build a mod item from the entry points found in the ztd file
PModItem PModDal::buildModFromEntryPoints(const QList<PFileData> &entryPoints, const QString &ztdPath) {
    PModItem mod;
    QFileInfo fileInfo(ztdPath);
    QString filename = fileInfo.fileName();
    QString location = fileInfo.absolutePath();
    QString fileSize  = QString::number(fileInfo.size());
    QString fileDate = fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");
    mod.setId(QUuid::createUuid().toString(QUuid::WithoutBraces));
    mod.setTitle(filename);
    mod.setAuthors({"Unknown"});
    mod.setDescription("No description found");
    mod.setEnabled(true);
    mod.setListed(true);
    mod.setCategory(""); // this will be determined later from rel path
    mod.setTags({"Unknown"});
    mod.setVersion("1.0.0");
    mod.setFilename(filename);
    mod.setLocation(location);
    mod.location = location;
    mod.setFileSize(fileSize);
    mod.setFileDate(fileDate);
    mod.setIconPaths({}); // this will be determined later
    mod.setOGLocation(ztdPath);
    mod.setSelected(false);
    mod.setDependencyId(QUuid::createUuid().toString(QUuid::WithoutBraces));

    return mod;
}

PModItem PModDal::buildDefaultMod(const QString &ztdPath) {
    PModItem mod;
    QFileInfo fileInfo(ztdPath);
    QString filename = fileInfo.fileName();
    QString location = fileInfo.absolutePath();
    QString fileSize  = QString::number(fileInfo.size());
    QString fileDate = fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");
    mod.setId(QUuid::createUuid().toString(QUuid::WithoutBraces));
    mod.setTitle(filename);
    mod.setAuthors({"Unknown"});
    mod.setListed(true);
    mod.setDescription("No description found");
    mod.setEnabled(true);
    mod.setCategory(""); // this will be determined later from rel path
    mod.setTags({"Unknown"});
    mod.setVersion("1.0.0");
    mod.setFilename(filename);
    mod.setLocation(location);
    mod.location = location;
    mod.setFileSize(fileSize);
    mod.setFileDate(fileDate);
    mod.setIconPaths({}); // this will be determined later
    mod.setOGLocation(ztdPath);
    mod.setSelected(false);
    mod.setDependencyId(QUuid::createUuid().toString(QUuid::WithoutBraces));

    return mod;
}

QMap<QString, QVariant> PModDal::generateFileData(const QString &filePath) {
    QFileInfo fileInfo(filePath);
    QMap<QString, QString> fileData;
    fileData.insert("filename", fileInfo.fileName());
    fileData.insert("size", QString::number(fileInfo.size()));
    fileData.insert("date", fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss"));
    fileData.insert("location", fileInfo.absolutePath());
    return fileData;
}

QStringList PModDal::generateTagsFromConfig(const PConfig &config) {
    QStringList tags = config.getAllKeys("member").value_or({});

    // Clean up the tags; format in proper case
    for (int i = 0; i < tags.size(); i++) {
        QString tag = tags[i];
        tag[0] = tag[0].toUpper();
        tags[i] = tag;
    }
    return tags;
}