#include "PModUIController.h"

PModUIController::PModUIController(QObject *parent, QStringList ztdList) : QAbstractListModel(parent), m_ztdList(ztdList)
{
    m_ztdList = ztdList;
}

int PModUIController::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_mods_list.size();
}

// The data method is used to fetch the data for a given index and role
// for a QAbstractListModel
QVariant PModUIController::data(const QModelIndex &index, int role) const
{
    qDebug () << "Fetching data for index: " << index.row() << ", role: " << role;
    if (index.isValid() && index.row() >= 0 && index.row() < m_mods_list.length())
    {
        QSharedPointer<PModItem> mod = m_mods_list[index.row()];
        qDebug() << "Fetching data for mod: " << mod->title();
        mod->getData(role); // this just translates the role to the data
    }

    // Return empty if the index is invalid
    qDebug() << "Invalid index: " << index.row() << ", returning empty QVariant";
    return QVariant();
}

// roleNames are a map that gives each enum role a string descriptor that
// can be called from QML
QHash<int, QByteArray> PModUIController::roleNames() const
{
    qDebug() << "PModUIController::roleNames() called";
    return PModItem::roleNames();
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
    // Reload mod from database
    beginResetModel();
    if (index >= 0 && index < m_mods_list.size())
    {
        // qDebug() << "Reloading mod: " << mod->modTitle();
        PDatabase db;
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
void PModUIController::reloadMod(QSharedPointer<PModItem> mod)
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
