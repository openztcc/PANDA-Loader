#ifndef PMODMODEL_H
#define PMODMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QSharedPointer>
#include "PModItem.h"
#include "../src/ldrs/PZTdMgr.h"
#include "../src/ldrs/PDatabaseMgr.h"
#include "../src/mgrs/PState.h"

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
        ModLocationRole,
        ModOgLocationRole,
        ModSelectedRole,
        ModVersionRole
    };

    explicit PModModel(QObject *parent = nullptr);

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
    void addState(PState *state);
    void replaceMod(QSharedPointer<PModItem> mod);

private:
    QList<QSharedPointer<PModItem>> m_mods_list;
    PState *m_state;
};

#endif // PMODMODEL_H
