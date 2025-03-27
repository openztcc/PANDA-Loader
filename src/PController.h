#ifndef PCONTROLLER_H
#define PCONTROLLER_H

/* This controller class will manipulate the state of the application. Mainly, it
concerns itself with the mods list and operations over other classes from the UI. */

// Qt includes
#include <QObject>
#include <QAbstractListModel>
#include <QList>
#include <QSharedPointer>
#include <QStringList>
#include <QVector>

// Project includes
#include "PModItem.h"
#include "PState.h"
#include "PZtdMgr.h"
#include "PDatabaseMgr.h"
#include "PConfigMgr.h"

// Third-party includes
#include "toml.hpp"

class PModItem;

class PController : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QObject* currentMod READ currentMod NOTIFY modSelected)
    Q_PROPERTY(int modCount READ modCount NOTIFY modAdded)

public:
    explicit PController(QObject *parent = nullptr);

    enum Role {
        ModTitleRole = Qt::UserRole + 1,
        ModAuthorRole,
        ModDescriptionRole,
        ModPathRole,
        ModEnabledRole,
        ModCategoryRole,
        ModTagsRole,
        ModIdRole
    };

    QSharedPointer<PModItem> currentlySelectedMod(QString modId) const;
    int modCount() const;
    void addMod(QSharedPointer<PModItem>);
    void removeMod(QSharedPointer<PModItem>);
    Q_INVOKABLE void selectMod(int index);
    void deselectMod();
    void clearSelection();
    void loadMods();
    void loadModsFromZTDs(const QStringList &ztdList);
    void addState(PState *state);
    Q_INVOKABLE void updateModList(QString orderBy, QString searchTerm);

    QObject* currentMod() const { return m_currentMod.data(); }
    Q_INVOKABLE void setCurrentMod(QObject *mod) { m_currentMod = currentlySelectedMod(mod->property("modId").toString()); emit modSelected(m_currentMod); }

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

signals:
    void modAdded(QSharedPointer<PModItem>);
    void modRemoved(QSharedPointer<PModItem>);
    void modSelected(QSharedPointer<PModItem>);
    void modDeselected();

private:
    QList<QSharedPointer<PModItem>> m_mods_list;
    QSharedPointer<PModItem> m_currentMod;
    PState *m_state;
};

#endif // PCONTROLLER_H
