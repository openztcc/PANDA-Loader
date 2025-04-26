#ifndef PModController_H
#define PModController_H

/* This controller class will manipulate the state of the application. Mainly, it
concerns itself with the mods list and operations over other classes from the UI. */

// Qt includes
#include <QObject>
#include <QList>
#include <QSharedPointer>
#include <QStringList>
#include <QVector>

// Project includes
#include "PState.h"
#include "PZtdMgr.h"
#include "PDatabaseMgr.h"
#include "PConfigMgr.h"
#include "PApeFile.h"

// Models
#include "PModModel.h"
#include "PModItem.h"

// Third-party includes
#include "toml.hpp"

class PModItem;

class PModController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* currentMod READ currentMod WRITE setCurrentMod NOTIFY currentModChanged)    
    Q_PROPERTY(QList<QObject*> selectedMods READ selectedMods NOTIFY selectedModsListUpdated)
    Q_PROPERTY(QObject* previousMod READ previousMod NOTIFY previousModChanged)
    Q_PROPERTY(int modCount READ modCount NOTIFY modAdded)
    Q_PROPERTY(QAbstractListModel* model READ model CONSTANT)

public:
    explicit PModController(QObject *parent = nullptr, PState *state = nullptr);

    PModModel* model() const { return m_model; }

    QSharedPointer<PModItem> getModAsObject(QString modId) const;
    int modCount() const;
    void addMod(QSharedPointer<PModItem>);
    Q_INVOKABLE void removeMod(QSharedPointer<PModItem>);
    Q_INVOKABLE bool setModEnabled(QSharedPointer<PModItem>, bool enabled);
    // Q_INVOKABLE void enableMod(QSharedPointer<PModItem>);
    Q_INVOKABLE void setSelectedModsEnabled(bool);
    // Q_INVOKABLE void enableSelected();
    Q_INVOKABLE void deleteSelected();
    Q_INVOKABLE void selectMod(int index);
    Q_INVOKABLE void deselectMod(int index);
    Q_INVOKABLE void selectAll();

    // effect controls
    Q_INVOKABLE void changeOpacity(QObject* qmlItem, float opacity);

    QObject* currentMod() const { return m_currentMod.data(); }
    Q_INVOKABLE void setCurrentMod(QObject* mod);
    Q_INVOKABLE void clearSelection();
    Q_INVOKABLE void addModToSelection(QObject* mod);
    Q_INVOKABLE QList<QObject*> selectedMods() const;
    // Q_INVOKABLE void removeFromSelectedMods(QObject* mod);
    QObject* previousMod() const { return m_previousMod.data(); }
    void reloadMod(QSharedPointer<PModItem> mod);
    Q_INVOKABLE void updateModList(QString orderBy, QString searchTerm);

signals:
    void modAdded(QSharedPointer<PModItem>);
    void modRemoved(QSharedPointer<PModItem>);
    void modSelected();
    void modDeselected();
    void previousModChanged();
    void currentModChanged();
    void selectedModsListUpdated(QList<QSharedPointer<PModItem>> mods);

private:
    QList<QSharedPointer<PModItem>> m_selected_mods;
    QSharedPointer<PModItem> m_currentMod;
    QSharedPointer<PModItem> m_previousMod;
    PState *m_state;
    PModModel *m_model;
};

#endif // PModController_H
