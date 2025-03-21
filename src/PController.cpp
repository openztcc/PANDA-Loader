#include "PController.h"

PController::PController(QObject *parent) : QAbstractListModel(parent), m_currentMod(nullptr), m_state(nullptr)
{
}

QSharedPointer<PModItem> PController::currentlySelectedMod() const
{
    return m_currentMod;
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
    emit modSelected(m_currentMod);
}

void PController::deselectMod()
{

}

void PController::clearSelection()
{
    emit modDeselected();
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
            case ModPathRole:
                return mod->modPath();
            case ModEnabledRole:
                return mod->modEnabled();
            case ModCategoryRole:
                return mod->modCategory();
            case ModTagsRole:
                return mod->modTags();
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
    roles[ModPathRole] = "modPath";
    roles[ModEnabledRole] = "modEnabled";
    roles[ModCategoryRole] = "modCategory";
    roles[ModTagsRole] = "modTags";

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
        mod->setmodPath(QUrl::fromLocalFile(query.value("path").toString()));
        mod->setmodEnabled(query.value("enabled").toBool());
        mod->setmodCategory(query.value("category").toString());
        mod->setmodTags(query.value("tags").toString());
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

        // Check if config exists
        if (!PZtdMgr::fileExistsInZtd(ztd, "meta.toml")) {
            qDebug() << "No meta config found in ztd: " << ztd;
            
            // Insert mod with blank values
            mod.title = "Unknown";
            mod.authors = {"Unknown"};
            mod.description = "No description found";
            mod.path = ztd;
            mod.enabled = true;
            mod.category = "Unknown";
            mod.tags = {"Unknown"};
            mod.version = "1.0.0";
            mod.mod_id = QUuid::createUuid().toString();
        }
        else {

            // Get meta config from ztd
            toml::table config = PConfigMgr::getMetaConfig(ztd);

            // Grab mod_id from config
            mod.mod_id = PConfigMgr::getKeyValue("mod_id", config);
            if (mod.mod_id.isEmpty()) {
                mod.mod_id = QUuid::createUuid().toString();
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

            mod.path = ztd;
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


        }

        db.insertMod(mod);

    }

    // close database
    db.closeDatabase();
    qDebug() << "Loaded mods from ZTDs";
}
