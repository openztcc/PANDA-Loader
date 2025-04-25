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

bool PController::setModEnabled(QSharedPointer<PModItem> mod, bool enabled)
{
    if (!m_state) {
        qCritical() << "PState is null, cannot toggle mod";
        return false;
    }

    // Check if the mod is already in the correct state
    if (mod->enabled() == enabled) {
        qDebug() << "Mod is already "<< (enabled ? "enabled" : "disabled") << ":" << mod->title();
        return false;
    }

    // File location variables

    QString filename = mod->filename();
    QString currentLocation = mod->location();
    qDebug() << "Current location:" << currentLocation;
    QString targetLocation;

    QString pandaHomePath = QDir::cleanPath(m_state->pandaHomePath());
    QString disabledDir = QDir::cleanPath(pandaHomePath + "/resources/mods/.disabled") + "/";
    QString originalLocation = mod->oglocation();
    qDebug() << "Original location:" << originalLocation;
    QString sourcePath, destPath;

    // Calculate correct paths
    if (enabled) {
        // disabled -> original location
        targetLocation = QDir::cleanPath(originalLocation) + "/";
        sourcePath = QDir::cleanPath(currentLocation + "/" + filename);
        destPath = QDir::cleanPath(targetLocation + filename);
    } else {
        // move to .disabled folder
        if (!QDir(disabledDir).exists()) {
            QDir().mkpath(disabledDir);
        }
        targetLocation = disabledDir;
        sourcePath = QDir::cleanPath(currentLocation + "/" + filename);
        destPath = QDir::cleanPath(disabledDir + filename);
    }

    qDebug() << (enabled ? "Enabling" : "Disabling") << "mod:" << mod->title();
    qDebug() << "Source:" << sourcePath << "-> Dest:" << destPath;

    if (!QFile::exists(sourcePath)) {
        qCritical() << "Source ZTD file does not exist:" << sourcePath;
        return false;
    }

    if (!PZtdMgr::moveFile(sourcePath, destPath)) {
        qCritical() << "Failed to move file from" << sourcePath << "to" << destPath;
        return false;
    }

    // Update the mod in the database
    PDatabaseMgr db;
    db.openDatabase();
    db.updateMod(mod->id(), "location", targetLocation);
    db.updateMod(mod->id(), "enabled", enabled ? "1" : "0");
    db.closeDatabase();

    // qml stuff
    if (QObject* component = mod->uiComponent()) {
        component->setProperty("opacity", enabled ? 1.0 : 0.5);
    }
    mod->setEnabled(enabled);
    mod->setLocation(targetLocation);

    return true;
}


void PController::reloadMod(QSharedPointer<PModItem> mod)
{
    m_model->reloadMod(mod);
}

void PController::setSelectedModsEnabled(bool enabled)
{
    QList<QSharedPointer<PModItem>> mods = m_selected_mods;
    QVector<QString> successful_mods;
    for (const auto& mod : mods) {
        if (setModEnabled(mod, enabled)) {
            successful_mods.append(mod->id());
        } else {
            qDebug() << "Failed to update enable status for:" << mod->title();
        }
    }

    PDatabaseMgr db;
    db.openDatabase();
    for (const auto& id : successful_mods) {
        QSharedPointer<PModItem> mod = db.getModByPk(id);
        if (mod) {
            // Update the mod in the model
            m_model->replaceMod(mod);
        } else {
            qDebug() << "Invalid mod returned at reload" << id;
        }
    }
    db.closeDatabase();
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

