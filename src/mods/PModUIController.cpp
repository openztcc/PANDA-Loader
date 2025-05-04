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
    int i = 0;
    for (auto &mod : mods) {
        mod->setmodIndex(i);
        m_mods_list->addItem(mod);
        i++;
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

bool PModUIController::isModSelected(int index) const {
    QSharedPointer<PModItem> modItem = m_mods_list->getItem(index);
    if (!modItem) {
        qDebug() << "Mod not found in list: " << index;
        return false;
    }

    // check if selected in db
    if (m_dataAccess->getFlag(modItem->id(), "is_selected")) {
        return true;
    }
    return false;
}

void PModUIController::setModSelected(int index, bool selected) {
    QSharedPointer<PModItem> mod = m_mods_list->getItem(index);
    if (!mod) {
        qDebug() << "Mod not found in list: " << index;
        return;
    }

    // update your internal data structures
    if (selected) {
        m_selected_mods.append(mod);
        mod->setSelected(true);
        emit modSelected();
    } else {
        m_selected_mods.removeOne(mod);
        mod->setSelected(false);
        emit modDeselected();
    }

    // set the selected flag in the database
    m_dataAccess->updateMod("mods", {{"is_selected", selected}}, {{"mod_id", mod->id()}});
    m_mods_list->setData(m_mods_list->getIndex(index), selected, PModItem::Role::ModSelectedRole);
    emit selectedModsListUpdated(m_selected_mods);
}

void PModUIController::clearSelection() {
    for (auto &mod : m_selected_mods) {
        setModSelected(m_mods_list->indexOf(mod), false);
    }
    m_selected_mods.clear();
    emit selectedModsListUpdated(m_selected_mods);
    emit forceModelUpdate();
}

void PModUIController::selectAllMods(bool selected) {
    for (int i = 0; i < m_mods_list->size(); i++) {
        setModSelected(i, selected);
    }
    emit selectedModsListUpdated(m_selected_mods);
}

void PModUIController::setCurrentMod(int index) {
    QSharedPointer<PModItem> modItem = m_mods_list->getItem(index);
    if (!modItem) {
        qDebug() << "Mod is not a valid PModItem, cannot set current mod.";
        return;
    }

    m_current_mod = modItem;
    emit currentModChanged();
}

void PModUIController::updateMod(int index) {
    QSharedPointer<PModItem> mod = m_mods_list->getItem(index);
    if (!mod) {
        qDebug() << "Mod not found in list: " << index;
        return;
    }

    m_mods_list->replaceItem(index, mod);
    emit forceModelUpdate();
}

void PModUIController::updateMod(int index, QSharedPointer<PModItem> mod) {
    if (!mod) {
        qDebug() << "Mod is not a valid PModItem, cannot update mod.";
        return;
    }

    m_mods_list->replaceItem(index, mod);
    emit forceModelUpdate();
}
