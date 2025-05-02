#include "PModUIController.h"

PModUIController::PModUIController(QObject *parent, QSharedPointer<PDatabase> db) : QObject(parent)
{
    // Initialize the model
    m_mods_list = QSharedPointer<PModList>::create(this);
    m_current_mod = nullptr;
    m_previous_mod = nullptr;
    m_selected_mods = QVector<QSharedPointer<PModItem>>();

    m_dataAccess = QSharedPointer<PModDataAccess>::create(this, db);
    m_loader = QSharedPointer<PModLoader>::create(this, m_dataAccess);
}

// --------------- Manage Mod List ------------------

void PModUIController::loadMods()
{
    QVector<QSharedPointer<PModItem>> mods = m_dataAccess->getAllMods("title", OrderBy::Ascending, {"listed", false});

    qDebug() << "Loaded mods from database: " << mods.size();

    //add to list
    for (const auto &mod : mods) {
        m_mods_list->addItem(mod);
    }

    qDebug() << "Mods list size: " << m_mods_list->size();
}

void PModUIController::reloadMod(int index)
{
    QSharedPointer<PModItem> listedMod = m_mods_list->getItem(index);
    if (!listedMod) {
        qDebug() << "Mod not found in list: " << index;
        return;
    }

    QVector<QSharedPointer<PModItem>> dbMod = m_dataAccess->searchMods(Operation::Select, "mod_id", listedMod->id());
    if (dbMod.isEmpty()) {
        qDebug() << "Mod not found in database: " << listedMod->id();
        return;
    }

    m_mods_list->replaceItem(index, dbMod[0]);
    qDebug() << "Reloaded mod: " << dbMod[0]->title();
}

// Updates the mod list based on the property (filter) and search term (value) for filtering and live search
void PModUIController::searchMods() 
{
    m_mods_list->replaceList(m_dataAccess->searchMods(Operation::Select, m_current_search_tags));
}

// Removes a mod from the list and database
void PModUIController::deleteMod(int index)
{
    // delete from list
    m_mods_list->removeItem(index);

    // delete from filesystem
    QSharedPointer<PModItem> mod = m_mods_list->getItem(index);
    QFile(mod->currentLocation() + "/" + mod->filename()).remove();
    
    // remove icons from filesystem
    m_loader->deleteIcons(mod->id());

    // remove from database
    if (mod) {
        m_dataAccess->deleteMod("mods", {{"mod_id", mod->id()}});
    } else {
        qDebug() << "Mod not found in list: " << index;
    }
}

// Adds a mod to the list and database
void PModUIController::addMod(QSharedPointer<PModItem> mod)
{
    m_mods_list->addItem(mod);
    m_dataAccess->insertMod(mod);
    qDebug() << "Added mod to list: " << mod->title();
}

bool PModUIController::isModSelected(QSharedPointer<PModItem> mod) const {
    // check if selected in db
    if (m_dataAccess->runOperation(Operation::Select, "mods", {{"mod_id", mod->id()}}).value("selected").toBool()) {
        return true;
    }
    return false;
}

void PModUIController::setModSelected(QSharedPointer<PModItem> mod, bool selected) {
    // check if the mod is already in the list
    if (m_selected_mods.contains(mod)) {
        qDebug() << "Mod already in selection: " << mod->title();
        return;
    }

    m_selected_mods.append(mod);
    mod->setSelected(selected);
    // set the selected flag in the database
    m_dataAccess->updateMod("mods", {{"is_selected", 1}}, {{"mod_id", mod->id()}});
    emit selectedModsListUpdated(m_selected_mods);
}
