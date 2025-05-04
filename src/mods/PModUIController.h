#ifndef PModUIController_H
#define PModUIController_H

// This is an implementation of the QAbstractListModel class and is only meant to
// serve as 1) a wrapper around the PModItem class, and 2) a datastructure for the QML
// view to display the list of mods. 

// Qt
#include <QObject>
#include <QAbstractListModel>
#include <QModelIndex>
#include <QSharedPointer>

// Project
#include "PModItem.h"
#include "PDatabase.h"
#include "PAppController.h"
#include "PDataList.h"
#include "PModLoader.h"
#include "PModDataAccess.h"

class PModItem;

class PModUIController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector<QSharedPointer<PModItem>> selectedMods READ selectedMods NOTIFY selectedModsListUpdated)
    Q_PROPERTY(QSharedPointer<PModItem> previousMod READ previousMod WRITE setPreviousMod NOTIFY previousModChanged)

    // Q_PROPERTY(QAbstractListModel* model READ model CONSTANT) TODO: fix this to be a QML model
public:

    explicit PModUIController(QObject *parent = nullptr, QSharedPointer<PDatabase> db = nullptr); // , QStringList ztdList = QStringList());

    // Model object
    QAbstractItemModel* model() const { return m_mods_list.get(); }

    Q_INVOKABLE void loadMods();
    void reloadMod(int index);
    Q_INVOKABLE void searchMods();
    void deleteMod(int index);
    void addMod(QSharedPointer<PModItem> mod);

    // Set select flags for the mod
    Q_INVOKABLE void setModSelected(int index, bool selected);
    Q_INVOKABLE bool isModSelected(int index) const;
    Q_INVOKABLE void clearSelection(int exceptIndex = -1);

    // Set disabled flags for the mod
    Q_INVOKABLE void setModDisabled(int index, bool disabled);
    Q_INVOKABLE bool isModDisabled(int index) const;
    
    void updateMod(int index);
    Q_INVOKABLE void updateMod(int index, QSharedPointer<PModItem> mod);
    Q_INVOKABLE void selectAllMods(bool selected);
    Q_INVOKABLE int selectedModsCount() const { return m_selected_mods.size(); }

    QVector<QSharedPointer<PModItem>> selectedMods() const { return m_selected_mods; }
    void setSelectedMods(QVector<QSharedPointer<PModItem>> selectedMods) { m_selected_mods = selectedMods; }

    // Current mod operations
    Q_INVOKABLE void setCurrentMod(int index);
    Q_INVOKABLE QObject* currentMod() const { return qobject_cast<QObject*>(m_current_mod.data()); }

    // Filter tag operations
    Q_INVOKABLE void addFilter(const QString &propertyName, const QString &searchTerm);
    Q_INVOKABLE void removeLastFilter();
    Q_INVOKABLE void clearFilters();

    QSharedPointer<PModItem> previousMod() const { return m_previous_mod; }
    void setPreviousMod(QSharedPointer<PModItem> mod) { m_previous_mod = mod; emit previousModChanged(); }


    // QML UI helpers (for visual cues)
    void updateOpacity(int index, bool enabled);

signals:
    void modAdded(QSharedPointer<PModItem>);
    void modRemoved(QSharedPointer<PModItem>);
    void modSelected();
    void modDeselected();
    void previousModChanged();
    void currentModChanged();
    void selectedModsListUpdated(QVector<QSharedPointer<PModItem>> mods);
    void forceModelUpdate();
private:
    // Mod list model
    QSharedPointer<PModList> m_mods_list;
    QSharedPointer<PModList> m_filtered_subset;
    QSharedPointer<PModList> m_backup_mods_list;

    QStringList m_ztdList;

    // Controller state
    QVector<QSharedPointer<PModItem>> m_selected_mods;
    QSharedPointer<PModItem> m_current_mod;
    QSharedPointer<PModItem> m_previous_mod;

    // Mod loader and data access
    QSharedPointer<PModDataAccess> m_dataAccess;
    QSharedPointer<PModLoader> m_loader;

    // Tags
    QList<QPair<QString, QVariant>> m_current_search_tags;

    QString m_disabled_location = QDir::homePath() + "/.panda/resources/mods/.disabled";

};

#endif // PModUIController_H
