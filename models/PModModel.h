#ifndef PMODMODEL_H
#define PMODMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QSharedPointer>
#include "PModItem.h"
#include "../src/PZTdMgr.h"
#include "../src/PDatabaseMgr.h"
#include "../src/PState.h"

class PModItem;

class PModModel : public QAbstractListModel
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
        ModLocationRole
    };

    explicit PModModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    void loadMods();
    Q_INVOKABLE void updateModList(QString orderBy, QString searchTerm);
    Q_INVOKABLE QList<QSharedPointer<PModItem>> modsList() const { return m_mods_list; }
    void removeMod(int index);
    void addMod(QSharedPointer<PModItem> mod);
    void addState(PState *state);

private:
    QList<QSharedPointer<PModItem>> m_mods_list;
    PState *m_state;
};

#endif // PMODMODEL_H
