#ifndef PModUIController_H
#define PModUIController_H

// This is an implementation of the QAbstractListModel class and is only meant to
// serve as 1) a wrapper around the PModItem class, and 2) a datastructure for the QML
// view to display the list of mods. 

// Qt
#include <QObject>
#include <QAbstractListModel>
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
    Q_PROPERTY(QSharedPointer<PModItem> currentMod READ currentMod WRITE setCurrentMod NOTIFY currentModChanged)    
    Q_PROPERTY(QVector<QSharedPointer<PModItem>> selectedMods READ selectedMods NOTIFY selectedModsListUpdated)
    Q_PROPERTY(QSharedPointer<PModItem> previousMod READ previousMod WRITE setPreviousMod NOTIFY previousModChanged)
    // Q_PROPERTY(QAbstractListModel* model READ model CONSTANT) TODO: fix this to be a QML model
public:

    explicit PModUIController(QObject *parent = nullptr, QSharedPointer<PDatabase> db = nullptr); // , QStringList ztdList = QStringList());

    // Model object
    QObject* model() const { return m_mods_list.get(); }

    void loadMods();
    void reloadMod(int index);
    Q_INVOKABLE void searchMods();
    void deleteMod(int index);
    void addMod(QSharedPointer<PModItem> mod);

    QVector<QSharedPointer<PModItem>> selectedMods() const { return m_selected_mods; }
    void setSelectedMods(QVector<QSharedPointer<PModItem>> selectedMods) { m_selected_mods = selectedMods; }

    void setCurrentMod(QSharedPointer<PModItem> mod) { m_current_mod = mod; emit currentModChanged(); }
    QSharedPointer<PModItem> currentMod() const { return m_current_mod; }

    QSharedPointer<PModItem> previousMod() const { return m_previous_mod; }
    void setPreviousMod(QSharedPointer<PModItem> mod) { m_previous_mod = mod; emit previousModChanged(); }

signals:
    void modAdded(QSharedPointer<PModItem>);
    void modRemoved(QSharedPointer<PModItem>);
    void modSelected();
    void modDeselected();
    void previousModChanged();
    void currentModChanged();
    void selectedModsListUpdated(QVector<QSharedPointer<PModItem>> mods);
private:
    QSharedPointer<PModList> m_mods_list;
    QStringList m_ztdList;
    QVector<QSharedPointer<PModItem>> m_selected_mods;
    QSharedPointer<PModItem> m_current_mod;
    QSharedPointer<PModItem> m_previous_mod;
    QSharedPointer<PModDataAccess> m_dataAccess;
    QSharedPointer<PModLoader> m_loader;
    QMap<QString, QVariant> m_current_search_tags;

};

#endif // PModUIController_H
