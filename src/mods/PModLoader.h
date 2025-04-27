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
#include "PDataList.h"

class PModItem;

class PModList : public QAbstractListModel
{
    Q_OBJECT
public:

    explicit PModList(QObject *parent = nullptr, QStringList ztdList = QStringList());

    int size(const QModelIndex &parent = QModelIndex()) const;
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
    PDataList<PModItem> m_mods_list;
    QStringList m_ztdList;
};

#endif // PMODLIST_H
