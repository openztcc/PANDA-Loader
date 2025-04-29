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
#include "PZTdMgr.h"
#include "PDatabase.h"
#include "PAppController.h"
#include "PDataList.h"
#include "PModLoader.h"
#include "PModDataAccess.h"

class PModItem;

class PModUIController : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QObject* currentMod READ currentMod WRITE setCurrentMod NOTIFY currentModChanged)    
    Q_PROPERTY(QList<QObject*> selectedMods READ selectedMods NOTIFY selectedModsListUpdated)
    Q_PROPERTY(QObject* previousMod READ previousMod NOTIFY previousModChanged)
    Q_PROPERTY(int modCount READ modCount NOTIFY modAdded)
    Q_PROPERTY(QAbstractListModel* model READ model CONSTANT)
public:

    explicit PModUIController(QObject *parent = nullptr, QStringList ztdList = QStringList());

    // Model object
    PModUIController* model() const { return m_model; }

    // Mod list properties
    int size(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    void loadMods();
    void reloadMod(int index);
    Q_INVOKABLE void updateModList(QString property, QString value);
    Q_INVOKABLE QList<QSharedPointer<PModItem>> modsList() const { return m_mods_list; }
    void deleteMod(int index);
    void addMod(QSharedPointer<PModItem> mod);
    void replaceMod(QSharedPointer<PModItem> mod);
signals:
    void modAdded(QSharedPointer<PModItem>);
    void modRemoved(QSharedPointer<PModItem>);
    void modSelected();
    void modDeselected();
    void previousModChanged();
    void currentModChanged();
    void selectedModsListUpdated(QList<QSharedPointer<PModItem>> mods);
private:
    PDataList<PModItem> m_mods_list;
    QStringList m_ztdList;
    QList<QSharedPointer<PModItem>> m_selected_mods;
    PModDataAccess m_dataAccess;
    PModLoader m_loader;

};

#endif // PModUIController_H
