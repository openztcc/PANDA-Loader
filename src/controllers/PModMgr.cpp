#include "PModMgr.h"

PModMgr::PModMgr(QObject *parent) : QAbstractListModel(parent)
{

}

int PModMgr::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_mods_list.size();
}

QVariant PModMgr::data(const QModelIndex &index, int role) const
{
    qDebug () << "Fetching data for index: " << index.row() << ", role: " << role;
    if (index.isValid() && index.row() >= 0 && index.row() < m_mods_list.length())
    {
        QSharedPointer<PModItem> mod = m_mods_list[index.row()];
        qDebug() << "Fetching data for mod: " << mod->title();
        switch ((Role) role)
        {
            case ModTitleRole:
                return mod->title();
            case ModAuthorRole:
                return mod->authors();
            case ModDescriptionRole:
                return mod->description();
            case ModEnabledRole:
                return mod->enabled();
            case ModCategoryRole:
                return mod->category();
            case ModTagsRole:
                return mod->tags();
            case ModIdRole:
                return mod->id();
            case ModFilenameRole:
                return mod->filename();
            case ModDependencyIdRole:
                return mod->dependencyId();
            case ModLocationRole:
                return mod->location();
            case ModIconPathsRole:
                return mod->iconpaths();
            case ModOgLocationRole:
                return mod->oglocation();
            case ModSelectedRole:
                return mod->selected();
            case ModVersionRole:
                return mod->version();
            case ModObjectRole:
                qDebug() << "Returning mod object: " << mod->title();
                return QVariant::fromValue(mod.data()); // return a whole mod object
        }
    }

    // Return empty if the index is invalid
    qDebug() << "Invalid index: " << index.row() << ", returning empty QVariant";
    return QVariant();
}

// Maps the roles to the data
QHash<int, QByteArray> PModMgr::roleNames() const
{
    qDebug() << "PModMgr::roleNames() called";
    QHash<int, QByteArray> roles;

    roles[ModTitleRole] = "title";
    roles[ModAuthorRole] = "authors";
    roles[ModDescriptionRole] = "description";
    roles[ModEnabledRole] = "enabled";
    roles[ModCategoryRole] = "category";
    roles[ModTagsRole] = "tags";
    roles[ModIdRole] = "id";
    roles[ModFilenameRole] = "filename";
    roles[ModDependencyIdRole] = "depId";
    roles[ModLocationRole] = "location";
    roles[ModObjectRole] = "instance"; // return a whole mod object
    roles[ModIconPathsRole] = "iconpaths";
    roles[ModOgLocationRole] = "oglocation";
    roles[ModSelectedRole] = "selected";
    roles[ModVersionRole] = "version";

    return roles;
}

// --------------- Manage Mod List ------------------

void PModMgr::loadMods()
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
        QSharedPointer<PModItem> mod = db.populateModItem(query);
        addMod(mod);
    }

    db.closeDatabase();
    qDebug() << "Loaded mods from database";
    endResetModel();
}

void PModMgr::reloadMod(int index)
{
    // Reload mod from database
    beginResetModel();
    if (index >= 0 && index < m_mods_list.size())
    {
        // qDebug() << "Reloading mod: " << mod->modTitle();
        PDatabaseMgr db;
        db.openDatabase();
        QString id = m_mods_list[index]->id();
        QSharedPointer<PModItem> newMod = db.getModByPk(id);
        // Update the mod in the list
        m_mods_list[index] = std::move(newMod);
        db.closeDatabase();
    }
    endResetModel();
}

// Reload mod from database
void PModMgr::reloadMod(QSharedPointer<PModItem> mod)
{
    // Get index from mod list
    int index = m_mods_list.indexOf(mod);
    if (index == -1)
    {
        qDebug() << "Mod not found in list: " << mod->title();
        return;
    }
    // Reload mod from database
    qDebug() << "Reloading mod: " << mod->title();
    reloadMod(index);
}

// Updates the mod list based on the property (filter) and search term (value) for filtering and live search
void PModMgr::updateModList(QString property, QString value) 
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
        QSharedPointer<PModItem> mod = db.populateModItem(query);
        addMod(mod);
    }
    db.closeDatabase();
    qDebug() << "Updated mod list from database";
    endResetModel();
}

// Removes a mod from the list and database
void PModMgr::removeMod(int index)
{
    // remove mod from list
    beginRemoveRows(QModelIndex(), index, index);
    m_mods_list.removeAt(index);
    endRemoveRows();
}

// Adds a mod to the list and database
void PModMgr::addMod(QSharedPointer<PModItem> mod)
{
    beginInsertRows(QModelIndex(), m_mods_list.size(), m_mods_list.size());
    qDebug() << "Adding mod to list: " << mod->title();
    m_mods_list.append(mod);
    endInsertRows();
    qDebug() << "New mod count: " << m_mods_list.size();
}

void PModMgr::addState(PAppController *state)
{
    m_state = state;
}

void PModMgr::replaceMod(QSharedPointer<PModItem> newMod) {
    int index = m_mods_list.indexOf(newMod);
    if (index == -1) {
        for (int i = 0; i < m_mods_list.size(); ++i) {
            if (m_mods_list[i]->id() == newMod->id()) {
                index = i;
                break;
            }
        }
    }

    if (index != -1) {
        beginResetModel();
        m_mods_list[index] = newMod;
        endResetModel();
    }
}