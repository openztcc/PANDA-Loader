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

void PModModel::reloadMod(int index)
{
    // Reload mod from database
    beginResetModel();
    if (index >= 0 && index < m_mods_list.size())
    {
        QSharedPointer<PModItem> mod = m_mods_list[index];
        qDebug() << "Reloading mod: " << mod->modTitle();
        PDatabaseMgr db;
        db.openDatabase();
        QSqlQuery query = db.getAllMods();
        while (query.next())
        {
            if (query.value("mod_id").toString() == mod->modId())
            {
                qDebug() << "Loading mod from database: " << query.value("title").toString();
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
            }
        }
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
    beginResetModel();
    qDebug() << "Reloading mod: " << mod->modTitle();
    reloadMod(index);
    endResetModel();
}

// Updates the mod list based on the order and search term for filtering and live search
void PModModel::updateModList(QString orderBy, QString searchTerm) 
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