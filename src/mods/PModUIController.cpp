#include "PModUIController.h"

PModUIController::PModUIController(QObject *parent, QStringList ztdList) : QAbstractListModel(parent), m_ztdList(ztdList), m_mods_list(parent)
{
    m_ztdList = ztdList;
}

// --------------- Manage Mod List ------------------

void PModUIController::loadMods()
{
    // Placeholder for loading mods; dynamic sqlite query later
    beginResetModel();
    // Clear existing mods
    m_mods_list.clear();

    // Open db
    PDatabase db;
    db.openDatabase();
    // Load mods from ztds
    db.loadModsFromZTDs(m_ztdList);

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

void PModUIController::reloadMod(int index)
{
    QSharedPointer<PModItem> listedMod = m_mods_list.getItem(index);
    if (!listedMod) {
        qDebug() << "Mod not found in list: " << index;
        return;
    }

    QVector<QSharedPointer<PModItem>> dbMod = m_dataAccess.searchMods(Operation::Select, "mod_id", listedMod->id());
    if (dbMod.isEmpty()) {
        qDebug() << "Mod not found in database: " << listedMod->id();
        return;
    }

    m_mods_list.replaceItem(index, dbMod[0]);
    qDebug() << "Reloaded mod: " << dbMod[0]->title();
}

// Updates the mod list based on the property (filter) and search term (value) for filtering and live search
void PModUIController::updateModList(QString property, QString value) 
{
    // Placeholder for updating mod list; dynamic sqlite query later
    beginResetModel();
    // Clear existing mods
    m_mods_list.clear();

    // Open db
    PDatabase db;
    db.open();

    QSqlQuery query;

    // Checks on orderBy and searchTerm
    if (property.isEmpty() && value.isEmpty()) {
        query = db.getAllMods();
    }
    else if (property.isEmpty() && !value.isEmpty()) {
        query = db.runQuery("title", value);
    }
    else {
        query = db.runQuery(property, value);
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
void PModUIController::removeMod(int index)
{
    // remove mod from list
    beginRemoveRows(QModelIndex(), index, index);
    m_mods_list.removeAt(index);
    endRemoveRows();
}

// Adds a mod to the list and database
void PModUIController::addMod(QSharedPointer<PModItem> mod)
{
    beginInsertRows(QModelIndex(), m_mods_list.size(), m_mods_list.size());
    qDebug() << "Adding mod to list: " << mod->title();
    m_mods_list.append(mod);
    endInsertRows();
    qDebug() << "New mod count: " << m_mods_list.size();
}

void PModUIController::replaceMod(QSharedPointer<PModItem> newMod) {
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
