#include "PModModel.h"

PModModel::PModModel(QObject *parent) : QAbstractListModel(parent)
{

}

int PModModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_mods_list.size();
}

QVariant PModModel::data(const QModelIndex &index, int role) const
{
    qDebug () << "Fetching data for index: " << index.row() << ", role: " << role;
    if (index.isValid() && index.row() >= 0 && index.row() < m_mods_list.length())
    {
        QSharedPointer<PModItem> mod = m_mods_list[index.row()];
        qDebug() << "Fetching data for mod: " << mod->modTitle();
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
            case ModOgLocationRole:
                return mod->oglocation();
            case ModSelectedRole:
                return mod->isSelected();
            case ModObjectRole:
                qDebug() << "Returning mod object: " << mod->modTitle();
                return QVariant::fromValue(mod.data()); // return a whole mod object
        }
    }

    // Return empty if the index is invalid
    qDebug() << "Invalid index: " << index.row() << ", returning empty QVariant";
    return QVariant();
}

// Maps the roles to the data
QHash<int, QByteArray> PModModel::roleNames() const
{
    qDebug() << "PModModel::roleNames() called";
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
    roles[ModIconPathsRole] = "modIconPaths";
    roles[ModOgLocationRole] = "modOgLocation";
    roles[ModSelectedRole] = "modSelected";

    return roles;
}

// --------------- Manage Mod List ------------------

void PModModel::loadMods()
{
    // Placeholder for loading mods; dynamic sqlite query later
    beginResetModel();
    // Clear existing mods
    m_mods_list.clear();

    // Load mods from directory
    QStringList ztdList = m_state->getZtdList();

    // Open db
    PDatabaseMgr db;
    db.openDatabase();
    // Load mods from ztds
    db.loadModsFromZTDs(ztdList);

    // Get all mods from database
    QSqlQuery query = db.getAllMods();

    // Iterate through the results and create PModItem objects
    while (query.next())
    {
        qDebug() << "Loading mod from database: " << query.value("title").toString();
        PModItem modItem = db.populateModItem(query);
        QSharedPointer<PModItem> mod = QSharedPointer<PModItem>::create(modItem);
        addMod(mod);
    }

    db.closeDatabase();
    qDebug() << "Loaded mods from database";
    endResetModel();
}

void PModModel::reloadMod(int index)
{
    // Reload mod from database
    beginResetModel();
    if (index >= 0 && index < m_mods_list.size())
    {
        // QSharedPointer<PModItem> mod = m_mods_list[index];
        // qDebug() << "Reloading mod: " << mod->modTitle();
        PDatabaseMgr db;
        db.openDatabase();
        PModItem modItem = db.populateModItem(query);
        QSharedPointer<PModItem> newMod = QSharedPointer<PModItem>::create(modItem);
        // Update the mod in the list
        m_mods_list[index] = std::move(newMod);
        db.closeDatabase();
    }
    endResetModel();
}

// Reload mod from database
void PModModel::reloadMod(QSharedPointer<PModItem> mod)
{
    // Get index from mod list
    int index = m_mods_list.indexOf(mod);
    if (index == -1)
    {
        qDebug() << "Mod not found in list: " << mod->modTitle();
        return;
    }
    // Reload mod from database
    qDebug() << "Reloading mod: " << mod->modTitle();
    reloadMod(index);
}

// Updates the mod list based on the property (filter) and search term (value) for filtering and live search
void PModModel::updateModList(QString property, QString value) 
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
    if (property.isEmpty() && value.isEmpty()) {
        query = db.getAllMods();
    }
    else if (property.isEmpty() && !value.isEmpty()) {
        query = db.queryMods("title", value);
    }
    else {
        query = db.queryMods(property, value);
    }

    // Iterate through the results and create PModItem objects
    while (query.next())
    {
        qDebug() << "Loading mod from database: " << query.value("title").toString();
        PModItem modItem = db.populateModItem(query);
        QSharedPointer<PModItem> mod = QSharedPointer<PModItem>::create(modItem);
        addMod(mod);
    }
    db.closeDatabase();
    qDebug() << "Updated mod list from database";
    endResetModel();
}

// Removes a mod from the list and database
void PModModel::removeMod(int index)
{
    // remove mod from list
    beginRemoveRows(QModelIndex(), index, index);
    m_mods_list.removeAt(index);
    endRemoveRows();
}

// Adds a mod to the list and database
void PModModel::addMod(QSharedPointer<PModItem> mod)
{
    beginInsertRows(QModelIndex(), m_mods_list.size(), m_mods_list.size());
    qDebug() << "Adding mod to list: " << mod->modTitle();
    m_mods_list.append(mod);
    endInsertRows();
    qDebug() << "New mod count: " << m_mods_list.size();
}

void PModModel::addState(PState *state)
{
    m_state = state;
}