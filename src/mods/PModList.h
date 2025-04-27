#ifndef PMODLIST_H
#define PMODLIST_H

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

class PModItem;

class PModList : public QAbstractListModel
{
    Q_OBJECT
public:

    enum Role {
        ModTitleRole = Qt::UserRole + 1,
        ModAuthorRole,
        ModDescriptionRole,
        ModEnabledRole,
        ModCategoryRole,
        ModTagsRole,
        ModIdRole,
        ModObjectRole,
        ModFilenameRole,
        ModIconPathsRole,
        ModDependencyIdRole,
        ModLocationRole,
        ModOgLocationRole,
        ModSelectedRole,
        ModVersionRole
    };

    explicit PModList(QObject *parent = nullptr, QStringList ztdList = QStringList());

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    void loadMods();
    void reloadMod(int index);
    void reloadMod(QSharedPointer<PModItem> mod);
    Q_INVOKABLE void updateModList(QString property, QString value);
    Q_INVOKABLE QList<QSharedPointer<PModItem>> modsList() const { return m_mods_list; }
    void removeMod(int index);
    void addMod(QSharedPointer<PModItem> mod);
    void replaceMod(QSharedPointer<PModItem> mod);

private:
    QList<QSharedPointer<PModItem>> m_mods_list;
    QStringList m_ztdList;
};

#endif // PMODLIST_H
