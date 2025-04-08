#include "PController.h"

PController::PController(QObject *parent) : QAbstractListModel(parent), m_currentMod(nullptr), m_state(nullptr)
{
}

QSharedPointer<PModItem> PController::getModAsObject(QString modId) const
{
    PDatabaseMgr db;
    db.openDatabase();

    PDatabaseMgr::PMod mod = db.getModByPk(modId);

    QSharedPointer<PModItem> modItem;
    modItem = QSharedPointer<PModItem>::create();
    modItem->setmodTitle(mod.title);
    modItem->setmodAuthor(mod.authors.join(", "));
    modItem->setmodDescription(mod.description);
    modItem->setmodEnabled(mod.enabled);
    modItem->setmodCategory(mod.category);
    modItem->setmodTags(mod.tags.join(", "));
    modItem->setmodId(mod.mod_id);
    modItem->setmodLocation(QUrl::fromLocalFile(mod.location));
    modItem->setmodFilename(mod.filename);
    modItem->setmodIconPaths(mod.iconpaths);
    modItem->setDependencyId(mod.dependencies[0].dependencyId);

    qDebug() << "Currently selected mod: " << modItem->modTitle();
    qDebug() << "With ID: " << modItem->modId();

    db.closeDatabase();

    return modItem;
}

int PController::modCount() const
{
    return m_mods_list.size();
}

void PController::addMod(QSharedPointer<PModItem> mod)
{
    beginInsertRows(QModelIndex(), m_mods_list.size(), m_mods_list.size());
    m_mods_list.append(mod);
    endInsertRows();

    emit modAdded(mod);
}

void PController::removeMod(QSharedPointer<PModItem> mod)
{
    int index = m_mods_list.indexOf(mod);
    if (index == -1)
    {
        return;
    }

    // delete mod from database
    PDatabaseMgr db;
    db.openDatabase();
    db.deleteMod(mod->modId());
    db.closeDatabase();

    // delete mod from filesystem
    QString location = mod->modLocation().toLocalFile();
    QString filename = mod->modFilename();
    QString ztdFilePath = location + "/" + filename;
    if (PZtdMgr::deleteFile(ztdFilePath)) {
        qDebug() << "Deleted ztd file: " << ztdFilePath;
    } else {
        qDebug() << "Failed to delete ztd file: " << ztdFilePath;
    }

    // remove mod from list
    beginRemoveRows(QModelIndex(), index, index);
    m_mods_list.removeAt(index);
    endRemoveRows();


    emit modRemoved(mod);
}

void PController::selectMod(int index)
{
    if (index < 0 || index >= m_mods_list.size())
    {
        return;
    }

    m_currentMod = m_mods_list[index];
    if (!m_currentMod)
    {
        qDebug() << "Current mod is null";
        return;
    }
    qDebug() << "Emitting modSelected signal: " << m_currentMod->modTitle();
    emit modSelected();
}

void PController::deselectMod(int index)
{
    if (index < 0 || index >= m_mods_list.size())
    {
        return;
    }

    // Check if the mod at the given index is the current mod
    if (m_mods_list[index] != m_currentMod)
    {
        qDebug() << "Selected mod is not the current mod, cannot deselect";
        return;
    }

    if (m_currentMod)
    {
        m_currentMod = nullptr;
        qDebug() << "Emitting modDeselected signal: " << m_currentMod->modTitle();
        emit modDeselected();
    }
    else
    {
        qDebug() << "Current mod is null, cannot deselect";
    }
}

void PController::clearSelection()
{
    // Clear the current mod and emit the deselected signal
    m_previousMod = m_currentMod;
    m_currentMod = nullptr;
    emit currentModChanged();
    emit modDeselected();
}

void PController::setCurrentMod(QObject* mod)
{ 
    // convert QObject to PModItem
    PModItem* modItem = qobject_cast<PModItem*>(mod);
    if (!modItem) {
        qDebug() << "Invalid mod object passed to setCurrentMod";
        return;
    }

    // find mod shared pointer in list
    for (const auto& sharedMod : m_mods_list) {
        if (sharedMod.data() == modItem) {
            if (m_currentMod != sharedMod) {
                // save the previous mod
                m_previousMod = m_currentMod;
                m_currentMod = sharedMod;

                // clear selected mods list and add current mod to it
                m_selected_mods.clear();
                m_selected_mods.append(m_currentMod);

                emit currentModChanged();
                emit previousModChanged();
            }
            return;
        }
    }

    qDebug() << "Mod not found in list: " << modItem->modTitle();
}

QList<QObject*> PController::selectedMods() const
{
    QList<QObject*> selectedModsList;
    for (const auto& mod : m_selected_mods) {
        selectedModsList.append(mod.data());
    }
    return selectedModsList;
}

void PController::addModToSelection(QObject* mod)
{
    // convert QObject to PModItem
    PModItem* modItem = qobject_cast<PModItem*>(mod);
    if (!modItem) {
        qDebug() << "Invalid mod object passed to addModToSelection";
        return;
    }

    // find mod shared pointer in list
    for (const auto& sharedMod : m_mods_list) {
        if (sharedMod.data() == modItem) {
            if (!m_selected_mods.contains(sharedMod)) {
                m_selected_mods.append(sharedMod);
                emit selectedModsListUpdated(m_selected_mods);
            }
            return;
        }
    }

    qDebug() << "Mod not found in list: " << modItem->modTitle();
}

int PController::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_mods_list.size();
}

QVariant PController::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && index.row() >= 0 && index.row() < m_mods_list.length())
    {
        QSharedPointer<PModItem> mod = m_mods_list[index.row()];
        switch ((Role) role)
        {
            case ModTitleRole:
                return mod->modTitle();
            case ModAuthorRole:
                return mod->modAuthor();
            case ModDescriptionRole:
                return mod->modDescription();
            case ModEnabledRole:
                return mod->modEnabled();
            case ModCategoryRole:
                return mod->modCategory();
            case ModTagsRole:
                return mod->modTags();
            case ModIdRole:
                return mod->modId();
            case ModFilenameRole:
                return mod->modFilename();
            case ModDependencyIdRole:
                return mod->dependencyId();
            case ModLocationRole:
                return mod->modLocation();
            case ModIconPathsRole:
                return mod->modIconPaths();
            case ModObjectRole:
                return QVariant::fromValue(mod.data()); // return a whole mod object
        }
    }

    // Return empty if the index is invalid
    return QVariant();
}

// Maps the roles to the data
QHash<int, QByteArray> PController::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[ModTitleRole] = "modTitle";
    roles[ModAuthorRole] = "modAuthor";
    roles[ModDescriptionRole] = "modDescription";
    roles[ModEnabledRole] = "modEnabled";
    roles[ModCategoryRole] = "modCategory";
    roles[ModTagsRole] = "modTags";
    roles[ModIdRole] = "modId";
    roles[ModFilenameRole] = "modFilename";
    roles[ModDependencyIdRole] = "modDependencyId";
    roles[ModLocationRole] = "modLocation";
    roles[ModObjectRole] = "modObject"; // return a whole mod object

    return roles;
}

void PController::loadMods()
{
    // Placeholder for loading mods; dynamic sqlite query later
    beginResetModel();
    // Clear existing mods
    m_mods_list.clear();

    // Load mods from directory
    QStringList ztdList = m_state->getZtdList();

    // Load mods from ztds
    loadModsFromZTDs(ztdList);

    // Open db
    PDatabaseMgr db;
    db.openDatabase();
    // Get all mods from database
    QSqlQuery query = db.getAllMods();

    // Iterate through the results and create PModItem objects
    while (query.next())
    {
        qDebug() << "Loading mod from database: " << query.value("title").toString();
        QSharedPointer<PModItem> mod = QSharedPointer<PModItem>::create();
        mod->setmodTitle(query.value("title").toString());
        mod->setmodAuthor(query.value("author").toString());
        mod->setmodDescription(query.value("description").toString());
        mod->setmodEnabled(query.value("enabled").toBool());
        mod->setmodCategory(query.value("category").toString());
        mod->setmodTags(query.value("tags").toString());
        mod->setmodId(query.value("mod_id").toString());
        mod->setmodFilename(query.value("filename").toString());
        mod->setmodIconPaths(query.value("iconpaths").toString().split(", ", Qt::SkipEmptyParts));
        mod->setDependencyId(query.value("dependency_id").toString());
        mod->setmodLocation(QUrl::fromLocalFile(query.value("location").toString()));
        addMod(mod);
    }

    db.closeDatabase();
    qDebug() << "Loaded mods from database";
    endResetModel();
}

void PController::addState(PState *state)
{
    m_state = state;
}

// Grabs mods from ZTDs and stores them in database
// TODO: Add any errors to a list of errors to display to user
// TODO: Add a check to see if mod already exists in database
// TODO: Add meta.toml file to ztd if it doesn't exist
// TODO: If meta.toml does not exist, add to list of errors for user
// TODO: Let user decide if it's a duplicate or not
void PController::loadModsFromZTDs(const QStringList &ztdList)
{
    // open database
    PDatabaseMgr db;
    if (!db.openDatabase()) {
        qDebug() << "Failed to open database for loading mods from ZTDs";
        return; // Failed to open database
    }

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
        if (db.searchMods("filename", filename).size() > 0) {
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
            if (db.doesModExist(mod.mod_id)) {
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
        }

        db.insertMod(mod);

    }

    // close database
    db.closeDatabase();
    qDebug() << "Loaded mods from ZTDs";
}

// Updates the mod list based on the order and search term for filtering and live search
void PController::updateModList(QString orderBy, QString searchTerm) 
{
    // Placeholder for updating mod list; dynamic sqlite query later
    beginResetModel();
    // Clear existing mods
    m_mods_list.clear();

    // Open db
    PDatabaseMgr db;
    db.openDatabase();

    QSqlQuery query;

    // Checks on orderBy and searchTerm
    if (orderBy.isEmpty() && searchTerm.isEmpty()) {
        query = db.getAllMods();
    }
    else if (orderBy.isEmpty() && !searchTerm.isEmpty()) {
        query = db.queryMods("title", searchTerm);
    }
    else {
        query = db.queryMods(orderBy, searchTerm);
    }

    // Iterate through the results and create PModItem objects
    while (query.next())
    {
        qDebug() << "Loading mod from database: " << query.value("title").toString();
        QSharedPointer<PModItem> mod = QSharedPointer<PModItem>::create();
        mod->setmodTitle(query.value("title").toString());
        mod->setmodAuthor(query.value("author").toString());
        mod->setmodDescription(query.value("description").toString());
        mod->setmodEnabled(query.value("enabled").toBool());
        mod->setmodCategory(query.value("category").toString());
        mod->setmodTags(query.value("tags").toString());
        mod->setmodId(query.value("mod_id").toString());
        mod->setmodIconPaths(query.value("iconpaths").toString().split(", ", Qt::SkipEmptyParts));
        mod->setmodFilename(query.value("filename").toString());
        mod->setmodLocation(QUrl::fromLocalFile(query.value("location").toString()));
        mod->setDependencyId(query.value("dependency_id").toString());
        addMod(mod);
    }
    db.closeDatabase();
    qDebug() << "Updated mod list from database";
    endResetModel();
}
