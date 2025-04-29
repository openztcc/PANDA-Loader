#include "PModLoader.h"

// Grabs mods from ZTDs and stores them in database
// TODO: Add any errors to a list of errors to display to user
// TODO: Add a check to see if mod already exists in database
// TODO: Add meta.toml file to ztd if it doesn't exist
// TODO: If meta.toml does not exist, add to list of errors for user
// TODO: Let user decide if it's a duplicate or not
void PModLoader::loadModsFromFile(const QStringList &ztdList)
{
    // Insert mods into database
    for (const QString &ztd : ztdList)
    {
        // Check if ztd already exists in database
        QVector<QSharedPointer<PModItem>> existingMods = m_dataAccess->searchMods(Operation::Select, {{"filename", QFileInfo(ztd).fileName()}});
        if (!existingMods.isEmpty()) {
            qDebug() << "ZTD already exists in database: " << QFileInfo(ztd).fileName();
            continue; // skip mod
        }
        
        // File loading and parsing
        PFile ztdFile(this, ztd, FileType::Zip); // Load the ztd file
        PFileData metaData = ztdFile.read("meta.toml"); // get the meta.toml at the root of the ztd (as a PFileData object)
        bool foundMeta = !metaData.data.isEmpty(); // did we find a meta.toml file in the ztd?

        // load the meta file so we can parse it
        PConfigMgr meta;
        if (foundMeta) {
            // Load the meta file and get the mod data
            meta.loadConfig(metaData.data);
        } else {
            qDebug() << "No meta config found in ztd: " << ztd;
        }

        // Get a list of the entrypoints in the ztd file. These are always in ucb, uca, ucs, or ai format.
        QVector<QSharedPointer<PFileData>> entryPoints = ztdFile.readAll({"animals/", "scenery/other/"}, {"ucb", "uca", "ucs", "ai"});
        int contentModsDetected = entryPoints.size();

        // if found meta file and more than 1 entrypoint, then this is 
        // a content pack. we will call it a collection.
        // for these kinds of mods, the 'collection' will be the name of the mod
        // and every 'mod' within the collection will be delisted in the modlist.
        // instead, they will be show in the infopane.
        QSharedPointer<PModItem> mod = QSharedPointer<PModItem>::create(nullptr);

        bool isCollection = contentModsDetected > 1;
        bool isSingleMod = contentModsDetected == 1;

        // ------------------------------------------------------------------- Build the base information for the mod

        if (foundMeta) { // meta.toml
            // Load the meta file and get the mod data
            PConfigMgr config(this, metaData.data);
            mod = buildModFromToml(config);
            mod->setIsCollection(true); // set the collection flag
        } else { // no meta file + no entrypoints. user will need to manually configure this mod.
            // if no meta file and no entrypoints, then this is a misc mod
            mod = buildDefaultMod(ztd);
            mod->setIsCollection(false);
        }

        // ------------------------------------------------------------------- Insert file data (size, date, etc.)
        generateFileData(ztd, mod);

        // ------------------------------------------------------------------- Set the tags for the mod
        if (isCollection) {
            QStringList tags = generateTagsFromConfig(meta);
            mod->setTags(tags);
        } else if (isSingleMod) {
            PConfigMgr cfg(nullptr, entryPoints[0]);
            QStringList tags = generateTagsFromConfig(cfg);
            mod->setTags(tags);
        } else {
            mod->setTags({}); // no tags found
        }

        // ------------------------------------------------------------------- Set mod status flags
        mod->setEnabled(true);
        mod->setSelected(false);
        mod->setListed(true);

        // ------------------------------------------------------------------- Set the icon paths for the mod
        QStringList iconPaths;
        QVector<QSharedPointer<PModItem>> collectionMods;
        if (isCollection) { // if it's a collection, we need to create mod listings for each mod in the entrypoint list
            // at this point, no entrypoint mod will have a meta.toml file, so most data will need to be be pulled from ini files
            collectionMods = buildCollectionMods(entryPoints, mod, ztdFile);
            mod->setCategory("Collection");
        } else {
            // if it's not a collection, then we can just set the icon paths from the entrypoint
            if (isSingleMod) {
                PConfigMgr epConfig(nullptr, entryPoints[0]);
                // ------------------------------------------------------------------- Set the category for the mod
                QString category = determineCategory(entryPoints[0]);
                mod->setCategory(category);

                QStringList epIconPaths = getIconPngPaths(epConfig, entryPoints[0], category, ztdFile);
                if (!epIconPaths.isEmpty()) {
                    mod->setIconPaths(epIconPaths);
                } else {
                    mod->setIconPaths({});
                }
            } else {
                // if no entrypoints, then set the icon paths to empty
                mod->setIconPaths({});
            }
        }

        m_dataAccess->insertMod(mod); // insert the mod into the database

        if (collectionMods.size() > 0) {
            for (auto collectionMod : collectionMods) {
                m_dataAccess->insertMod(collectionMod); // insert the collection mods into the database
            }
        }


                    // // ---------------------- Get Description, Title -----------------------

                    // if (category == "Animals") {}
                    //     QString description = config.getValue("cLongHelp", "1033").toString();
                    //     QString title = config.getValue("cName", "1033").toString();
                    //     QString category = determineCategory(config);
                    //     if (category.isEmpty()) {
                    //         category = "Unknown";
                    //     }
    }

    // // close database
    // db.closeDatabase();
    qDebug() << "Loaded mods from ZTDs";
}

// Determine the category of the mod based on the file extension and path
// Possible categories are: Building, Scenery, Animals, misc, Unknown
QString PModLoader::determineCategory(const PFileData &fileData) {
    if (fileData.data.size() == 0) {
        return "Unknown";
    }

    QString ext = fileData.ext;

    if (ext == "ucb") {
            return "Building";
    } else if (ext == "ucs") {
            return "Scenery";
    } else if (ext == "uca") {
            return "Animals";
    } else if (ext == "ai") {
        QStringList pathParts = fileData.path.split("/");
        // category is always the first part of the path
        QString category = pathParts[0];
        // return proper case for category
        return category.toUpper().left(1) + category.mid(1).toLower();
    } else {
            return "Unknown";
    }
}

QVector<QSharedPointer<PModItem>> PModLoader::buildCollectionMods(QList<PFileData> entryPoints, QSharedPointer<PModItem> mod, PFile &ztd) {
    QVector<QSharedPointer<PModItem>> collectionMods;
    for (const PFileData &entryPoint : entryPoints) {
        QSharedPointer<PModItem> epMod = QSharedPointer<PModItem>::create(nullptr);
        PConfigMgr epConfig(nullptr, entryPoint);
        // first, copy some base mod data from the collection mod
        epMod->setAuthors(mod->authors());
        epMod->setId(mod->id());
        epMod->setVersion(mod->version());

        epMod->setCollectionId(mod->id());

        epMod->setFilename(mod->filename());
        epMod->setCurrentLocation(mod->currentLocation());
        epMod->setOriginalLocation(mod->currentLocation());
        epMod->setDisabledLocation("");
        epMod->setFileSize(mod->fileSize());

        // generate the rest of the data
        QString epCategory = determineCategory(entryPoint);
        epMod->setCategory(epCategory);
        epMod->setTags(generateTagsFromConfig(epConfig));
        epMod->setEnabled(true);
        epMod->setSelected(false);
        epMod->setListed(false); // since this is a collection item, we don't want to show it in the modlist

        QStringList epIconPngPaths = getIconPngPaths(epConfig, epCategory, ztd);
        if (!epIconPngPaths.isEmpty()) {
            epMod->setIconPaths(epIconPngPaths);
        } else {
            epMod->setIconPaths({});
        }

        // get the description and title from the config file
        QString epDescription = determineDescription(epConfig, epCategory);
        QString epTitle = determineTitle(epConfig, epCategory);

        epMod->setDescription(epDescription);
        epMod->setTitle(epTitle);
        collectionMods.append(epMod);
    }

}

// Determine the category of the mod based on the meta.toml file in the root of the ztd
QSharedPointer<PModItem> PModLoader::buildModFromToml(PConfigMgr &config) {
    QSharedPointer<PModItem> mod = QSharedPointer<PModItem>::create(nullptr);

    // values from TOML file
    mod->setId(config.getValue("mod_id", "").toString());
    mod->setTitle(config.getValue("name", "").toString());
    mod->setAuthors(config.getValue("authors", "").toStringList());
    mod->setDescription(config.getValue("description", "").toString());
    mod->setVersion(config.getValue("version", "").toString());
    mod->setLink(config.getValue("link", "").toString());

    // if dependency table exists, then add the dependency id to the mod and add to db
    QMap<QString, QVariant> depTable = config.getValue("dependencies", "").toMap();
    if (!depTable.isEmpty()) {
        for (const auto &dep : depTable) {
            PDepDataAccess dependency(mod->id(), dep);
            if (dependency.isValid()) {
                mod->setDependencyId(dependency.id());
            }
        }
    }
    // if no dependency table, then set the dependency id to None
    else {
        mod->setDependencyId("None");
    }
    mod->setDependencyId(config.getValue("dep_id", "").toString());

    return mod;
}

QSharedPointer<PModItem> PModLoader::buildDefaultMod(const QString &ztdPath) {
    QSharedPointer<PModItem> mod = QSharedPointer<PModItem>::create(nullptr);
    mod->setId(QUuid::createUuid().toString(QUuid::WithoutBraces));
    mod->setTitle(QFileInfo(ztdPath).fileName());
    mod->setAuthors({"Unknown"});
    mod->setDescription("No description found");
    mod->setVersion("1.0.0");

    return mod;
}

void PModLoader::generateFileData(const QString &filePath, QSharedPointer<PModItem> mod) {
    QFileInfo fileInfo(filePath);
    mod->setFilename(fileInfo.fileName());
    mod->setFileSize(QString::number(fileInfo.size()));
    mod->setFileDate(fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss"));
    mod->setCurrentLocation(fileInfo.absolutePath());
    mod->setOriginalLocation(fileInfo.absolutePath());
    mod->setDisabledLocation("");
}

QStringList PModLoader::generateTagsFromConfig(PConfigMgr &config) {
    QStringList tags = config.getAllKeys("member");

    // Clean up the tags; format in proper case
    for (int i = 0; i < tags.size(); i++) {
        QString tag = tags[i];
        tag[0] = tag[0].toUpper();
        tags[i] = tag;
    }
    return tags;
}

QStringList PModLoader::getIconPngPaths(PConfigMgr &config, const QString &category, PFile &ztd) {
    QStringList aniPaths = getIconAniPaths(config, category);
    QStringList iconPaths = getIconPaths(aniPaths, ztd);
    return iconPaths;
}

// TODO: Expand this to include more categories
QStringList PModLoader::getIconAniPaths(PConfigMgr &config, const QString &category) {
    QStringList iconAniPaths;
    if (category == "Animals") {
        // animals have 1-2 icon ani paths
        // [m/Icon] and [f/Icon]. Sometimes only one exists.
        QString aniPathF = config.getValue("f/Icon", "Icon").toString();
        QString aniPathM = config.getValue("m/Icon", "Icon").toString();
        if (!aniPathF.isEmpty()) {
            iconAniPaths.append(aniPathF);
        }
        if (!aniPathM.isEmpty()) {
            iconAniPaths.append(aniPathM);
        }
        return iconAniPaths;
    } else if (category == "Building" || category == "Scenery") {
        // objects have just 1 icon ani path
        return {config.getValue("Icon", "Icon").toString()};
    } else {
        return QStringList();
    }
}

QStringList PModLoader::getIconPaths(const QStringList &aniPaths, PFile &ztd) {
    QVector<QSharedPointer<PFileData>> graphicFileData;

    for (const QString &aniPath : aniPaths) {
        // get the ani path and generate the icon path
        PFileData aniData = ztd.read(aniPath);
        PConfigMgr aniConfig(nullptr, aniData);

        QString aniFileName = aniPath.section("/").last(); // get the ani file name from the rel path

        QString iconPath = buildGraphicPath(aniConfig);
        iconPaths.append(iconPath);
    }

    QStringList pngPaths;
    for (const QString &path : iconPaths) {
        // get the ani path and generate the icon path
        QStringList pngs = PApeFile::generateGraphicsAsPng(iconPaths, type);
        for (const QString &png : pngs) {
            pngPaths.append(png);
        }
    }
    return pngPaths;
}

// ani files provide the directory structure in key/value pairs
// [animation]
// dir0 = "animals"
// dir1 = "ferret"
// dir2 = "other"
// animation = "n" or "N"
// the number of dirN keys isn't guaranteed to be the same for all ani files
QString PModLoader::buildGraphicPath(PConfigMgr &aniFile) {
    // clamp limit to 10 directory parts
    QString iconPath;
    for (int i = 0; i < 10; i++) {
        QString dirKey = "dir" + QString::number(i);
        QString dirValue = aniFile.getValue(dirKey, "animation").toString();
        if (dirValue.isEmpty()) {
            break; // no more directories to add
        }
        iconPath += dirValue + "/";
    }
    // remove the last "/" from the path
    iconPath.chop(1);

    return iconPath;
}
