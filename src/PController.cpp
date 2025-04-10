#include "PController.h"

PController::PController(QObject *parent, PState *state)
{
    m_model = new PModModel(this);
    m_state = state;
    m_model->addState(state);
    m_model->loadMods();
}

QSharedPointer<PModItem> PController::getModAsObject(QString id) const
{
    PDatabaseMgr db;
    db.openDatabase();

    QSharedPointer<PModItem> modItem = db.getModByPk(id);

    qDebug() << "Currently selected mod: " << modItem->title();
    qDebug() << "With ID: " << modItem->id();

    db.closeDatabase();

    return modItem;
}

int PController::modCount() const
{
    return m_model->modsList().size();
}

void PController::addMod(QSharedPointer<PModItem> mod)
{
    m_model->addMod(mod);
    emit modAdded(mod);
}

void PController::removeMod(QSharedPointer<PModItem> mod)
{
    int index = m_model->modsList().indexOf(mod);
    if (index == -1)
    {
        return;
    }

    // delete icons associated with mod
    if (PGraphicsMgr::deleteIcons(mod->id())) {
        qDebug() << "Deleted icons for mod: " << mod->id();
    } else {
        qDebug() << "Failed to delete icons for mod: " << mod->id();
    }

    // delete mod from database
    PDatabaseMgr db;
    db.openDatabase();
    db.deleteMod(mod->id());
    db.closeDatabase();

    // delete mod from filesystem
    QString location = mod->location();
    QString filename = mod->filename();
    QString ztdFilePath = location + "/" + filename;
    if (PZtdMgr::deleteFile(ztdFilePath)) {
        qDebug() << "Deleted ztd file: " << ztdFilePath;
    } else {
        qDebug() << "Failed to delete ztd file: " << ztdFilePath;
    }

    // delete mod from model
    m_model->removeMod(index);

    emit modRemoved(mod);
}

// Deletes selected mods from the database and filesystem
void PController::deleteSelected()
{
    for (const auto& mod : m_selected_mods) {
        removeMod(mod);
    }
    m_selected_mods.clear();
    emit selectedModsListUpdated(m_selected_mods);
}

void PController::disableMod(QSharedPointer<PModItem> mod)
{
    // File location variables
    QString fileBasePath;
    QString filename;

    // Get the game path from the state
    QString pandaHomePath = QDir::cleanPath(m_state->settings()->pandaHomePath());
    QString disabledDir;

    // Calculate correct paths
    qDebug() << "Disabling mod: " << mod->title();
    if (!m_state) {
        qCritical() << "PState is null, cannot disable mod";
        return;
    } else {
        fileBasePath = QDir::cleanPath(m_state->getGamePath());
        filename = mod->filename();
        // needs trailing slash because the cleanpath removes it
        disabledDir = QDir::cleanPath(pandaHomePath + "/resources/mods/.disabled") + "/";
    }

    // Sanity checks
    qDebug() << "Checking if disabled mods folder exists: " << disabledDir;
    if (!QDir(disabledDir).exists()) {
        QDir().mkpath(disabledDir);
    } else {
        qDebug() << "Disabled mods folder already exists: " << disabledDir;
    }
    qDebug() << "Moving ztd file to disabled mods folder: " << disabledDir;

    // Determine final locations for file
    QString ztdFilePath = QDir::cleanPath(mod->location() + "/" + filename);
    QString newZtdFilePath = QDir::cleanPath(disabledDir + filename);

    // Final sanity checks
    qDebug() << "ZTD file path: " << ztdFilePath;
    if (!QFile::exists(ztdFilePath)) {
        qCritical() << "ZTD file path does not exist:" << ztdFilePath;
        return;
    }    

    // Move file to disabled mods folder
    if (PZtdMgr::moveFile(ztdFilePath, newZtdFilePath)) {
        qDebug() << "Moved ztd file to disabled mods folder: " << newZtdFilePath;
    } else {
        qDebug() << "Failed to move ztd file: " << ztdFilePath;
        return;
    }

    // Update the mod in the database
    PDatabaseMgr db;
    db.openDatabase();
    db.updateMod(mod->id(), "location", disabledDir);
    db.updateMod(mod->id(), "enabled", "0");
    db.closeDatabase();

    // qml stuff
    if (QObject* component = mod->uiComponent()) {
        component->setProperty("opacity", 0.5);
    }
    mod->setEnabled(false);

    // Reload the mod
    reloadMod(mod);

}

void PController::reloadMod(QSharedPointer<PModItem> mod)
{
    m_model->reloadMod(mod);
}

void PController::disableSelected()
{
    for (const auto& mod : m_selected_mods) {
        disableMod(mod);
    }
}

void PController::changeOpacity(QObject* qmlItem, float opacity)
{
    if (qmlItem) {
        qmlItem->setProperty("opacity", opacity);
        qDebug() << "Set opacity for qml item.";
    } else {
        qDebug() << "QML item is null";
    }
}

void PController::selectMod(int index)
{
    if (index < 0 || index >= m_model->modsList().size())
    {
        return;
    }

    m_currentMod = m_model->modsList()[index];
    if (!m_currentMod)
    {
        qDebug() << "Current mod is null";
        return;
    }
    qDebug() << "Emitting modSelected signal: " << m_currentMod->title();
    emit modSelected();
}

void PController::deselectMod(int index)
{
    if (index < 0 || index >= m_model->modsList().size())
    {
        return;
    }

    // Check if the mod at the given index is the current mod
    if (m_model->modsList()[index] != m_currentMod)
    {
        qDebug() << "Selected mod is not the current mod, cannot deselect";
        return;
    }

    if (m_currentMod)
    {
        m_currentMod = nullptr;
        qDebug() << "Emitting modDeselected signal: " << m_currentMod->title();
        emit modDeselected();
    }
    else
    {
        qDebug() << "Current mod is null, cannot deselect";
    }
}

void PController::clearSelection()
{
    // Clear the current mod and emit the deselected signal
    m_previousMod = m_currentMod;
    m_currentMod = nullptr;
    emit currentModChanged();
    emit modDeselected();
}

void PController::setCurrentMod(QObject* mod)
{ 
    // convert QObject to PModItem
    PModItem* modItem = qobject_cast<PModItem*>(mod);
    if (!modItem) {
        qDebug() << "Invalid mod object passed to setCurrentMod";
        return;
    }

    // find mod shared pointer in list
    for (const auto& sharedMod : m_model->modsList()) {
        if (sharedMod.data() == modItem) {
            if (m_currentMod != sharedMod) {
                // save the previous mod
                m_previousMod = m_currentMod;
                m_currentMod = sharedMod;

                // clear selected mods list and add current mod to it
                m_selected_mods.clear();
                m_selected_mods.append(m_currentMod);

                emit currentModChanged();
                emit previousModChanged();
            }
            return;
        }
    }

    qDebug() << "Mod not found in list: " << modItem->title();
}

QList<QObject*> PController::selectedMods() const
{
    QList<QObject*> selectedModsList;
    for (const auto& mod : m_selected_mods) {
        selectedModsList.append(mod.data());
    }
    return selectedModsList;
}

void PController::addModToSelection(QObject* mod)
{
    // convert QObject to PModItem
    PModItem* modItem = qobject_cast<PModItem*>(mod);
    if (!modItem) {
        qDebug() << "Invalid mod object passed to addModToSelection";
        return;
    }

    // find mod shared pointer in list
    for (const auto& sharedMod : m_model->modsList()) {
        if (sharedMod.data() == modItem) {
            if (!m_selected_mods.contains(sharedMod)) {
                m_selected_mods.append(sharedMod);
                emit selectedModsListUpdated(m_selected_mods);
            }
            return;
        }
    }

    qDebug() << "Mod not found in list: " << modItem->title();
}

void PController::updateModList(QString orderBy, QString searchTerm)
{
    m_model->updateModList(orderBy, searchTerm);
}

void PController::selectAll()
{
    m_selected_mods.clear();
    for (const auto& mod : m_model->modsList()) {
        m_selected_mods.append(mod);
    }
    emit selectedModsListUpdated(m_selected_mods);
}
