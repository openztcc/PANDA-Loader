#include "PModUIController.h"

PModUIController::PModUIController(QObject *parent, QStringList ztdList) : QAbstractListModel(parent), m_ztdList(ztdList), m_mods_list(parent)
{
    m_ztdList = ztdList;

    // load mods from file here (using PModLoader)
}

// --------------- Manage Mod List ------------------

void PModUIController::loadMods()
{
    QVector<QSharedPointer<PModItem>> mods = m_dataAccess.getAllMods(OrderBy::Ascending, {"listed", true});

    //add to list
    for (const auto &mod : mods) {
        m_mods_list.addItem(mod);
    }
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
void PModUIController::searchMods(QString property, QString value) 
{
    m_dataAccess.searchMods(Operation::Select, property, value);
}

// Removes a mod from the list and database
void PModUIController::deleteMod(int index)
{
    // delete from list
    m_mods_list.removeItem(index);

    // delete from database
    QSharedPointer<PModItem> mod = m_mods_list.getItem(index);
    if (mod) {
        m_dataAccess.deleteMod("mods", {{"mod_id", mod->id()}});
    } else {
        qDebug() << "Mod not found in list: " << index;
    }
}

// Adds a mod to the list and database
void PModUIController::addMod(QSharedPointer<PModItem> mod)
{
    m_mods_list.addItem(mod);
    m_dataAccess.insertMod(*mod);
    qDebug() << "Added mod to list: " << mod->title();
}
