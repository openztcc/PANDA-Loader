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
#include "../models/PModItem.h"
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
    Q_PROPERTY(QObject* currentMod READ currentMod WRITE setCurrentMod NOTIFY currentModChanged)    
    Q_PROPERTY(QObject* previousMod READ previousMod NOTIFY previousModChanged)
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
        ModIdRole,
        ModObjectRole
    };

    QSharedPointer<PModItem> getModAsObject(QString modId) const;
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
    Q_INVOKABLE void setCurrentMod(QObject* mod);

    QObject* previousMod() const { return m_previousMod.data(); }

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

signals:
    void modAdded(QSharedPointer<PModItem>);
    void modRemoved(QSharedPointer<PModItem>);
    void modSelected();
    void modDeselected();
    void previousModChanged();
    void currentModChanged();

private:
    QList<QSharedPointer<PModItem>> m_mods_list;
    QList<QObject*> m_selected_mods;
    QSharedPointer<PModItem> m_currentMod;
    QSharedPointer<PModItem> m_previousMod;
    PState *m_state;
};

#endif // PCONTROLLER_H
