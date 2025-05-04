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

    // update backup list
    m_backup_mods_list = m_mods_list;

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
    // build a qmap from the current search tags
    QMap<QString, QVariant> searchTags;
    for (const auto &tag : m_current_search_tags) {
        QString filter = tag.first;
        QVariant searchTerm = tag.second;
        searchTags.insert(filter, searchTerm);
    }
    m_mods_list->replaceList(m_dataAccess->searchMods(Operation::Select, searchTags));
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

    mod->setSelected(selected);

    // update your internal data structures
    if (selected) {
        m_selected_mods.append(mod);
        emit modSelected();
    } else {
        m_selected_mods.removeOne(mod);
        emit modDeselected();
    }

    // set the selected flag in the database
    m_dataAccess->updateMod("mods", {{"is_selected", selected}}, {{"mod_id", mod->id()}});
    m_mods_list->setData(m_mods_list->getIndex(index), selected, PModItem::Role::ModSelectedRole);
    emit selectedModsListUpdated(m_selected_mods);
}

void PModUIController::clearSelection(int exceptIndex) {
    for (const auto &mod : m_selected_mods) {
        int index = m_mods_list->indexOf(mod);
        if (index != -1 && index != exceptIndex) {
            // remove from selected mods list
            setModSelected(index, false);
        }
    }

    m_selected_mods.clear();

    if (exceptIndex != -1) {
        setModSelected(exceptIndex, true);
    }
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

void PModUIController::setModDisabled(int index, bool disabled) {
    QSharedPointer<PModItem> mod = m_mods_list->getItem(index);
    if (!mod) {
        qDebug() << "Mod not found in list: " << index;
        return;
    }

    qDebug() << "Attempt to set enable status to: " << disabled << " for mod: " << mod->title();
    mod->setEnabled(!disabled);

    if (disabled) {
        qDebug() << "Mod is disabled, moving to disabled location: " << m_disabled_location + "/" + mod->filename();
        mod->setCurrentLocation(m_disabled_location + "/" + mod->filename());
    } else {
        qDebug() << "Mod is enabled, moving to original location: " << mod->originalLocation() + "/" + mod->filename();
        mod->setCurrentLocation(mod->originalLocation());
    }

    qDebug() << "Updating mod in database: " << mod->title() << " with enabled status: " << !disabled;
    m_dataAccess->updateMod("mods", {{"enabled", !disabled}, {"current_location", mod->currentLocation()}}, {{"mod_id", mod->id()}});
    qDebug() << "Updated mod in database. Now updating model.";
    m_mods_list->setData(m_mods_list->getIndex(index), !disabled, PModItem::Role::ModEnabledRole);
    m_mods_list->setData(m_mods_list->getIndex(index), mod->currentLocation(), PModItem::Role::ModCurrentLocationRole);

    // Move mod to disabled location (or original location)
    if (disabled) {
        qDebug() << "Moving mod to disabled location: " << m_disabled_location + "/" + mod->filename();
        QFile::rename(mod->originalLocation() + "/" + mod->filename(), m_disabled_location + "/" + mod->filename());
    } else {
        qDebug() << "Moving mod to original location: " << mod->originalLocation() + "/" + mod->filename();
        QFile::rename(m_disabled_location + "/" + mod->filename(), mod->originalLocation() + "/" + mod->filename());
    }

    qDebug() << "Mod moved successfully. Now updating UI component opacity.";
}

bool PModUIController::isModDisabled(int index) const {
    QSharedPointer<PModItem> mod = m_mods_list->getItem(index);
    if (!mod) {
        qDebug() << "Mod not found in list: " << index;
        return false;
    }

    // check if disabled in db
    if (m_dataAccess->getFlag(mod->id(), "enabled")) {
        return false;
    }
    return true;
}

void PModUIController::updateOpacity(int index, bool enabled) {
    QSharedPointer<PModItem> mod = m_mods_list->getItem(index);
    if (!mod) {
        qDebug() << "Mod not found in list: " << index;
        return;
    }

    if (!mod->uiComponent()) {
        qDebug() << "Mod UI component is null, cannot update opacity.";
        return;
    }
    mod->uiComponent()->setProperty("opacity", enabled ? 1.0 : 0.9);
}

// --------------- Filter and Search ------------------

void PModUIController::addFilter(const QString &propertyName, const QString &searchTerm) {
    qDebug() << "Adding filter: " << propertyName << " with search term: " << searchTerm;
    m_current_search_tags.append({propertyName, searchTerm});
    searchMods();
}

void PModUIController::removeLastFilter() {
    m_current_search_tags.removeLast();
    searchMods();
}

void PModUIController::clearFilters() {
    // restore the backup list
    m_mods_list = m_backup_mods_list;
    m_current_search_tags.clear();
}
