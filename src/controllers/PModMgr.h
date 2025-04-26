#ifndef PModMgr_H
#define PModMgr_H

// Qt
#include <QObject>
#include <QAbstractListModel>
#include <QSharedPointer>

// Project
#include "PModItem.h"
#include "PZTdMgr.h"
#include "PDatabaseMgr.h"
#include "PAppController.h"

class PModItem;

class PModMgr : public QAbstractListModel
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

    explicit PModMgr(QObject *parent = nullptr);

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
    void addState(PAppController *state);
    void replaceMod(QSharedPointer<PModItem> mod);

private:
    QList<QSharedPointer<PModItem>> m_mods_list;
    PAppController *m_state;
};

#endif // PModMgr_H
