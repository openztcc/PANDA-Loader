#ifndef PCONTROLLER_H
#define PCONTROLLER_H

/* This controller class will manipulate the state of the application. Mainly, it
concerns itself with the mods list and operations over other classes from the UI. */

#include <QObject>
#include <QAbstractListModel>
#include <QList>
#include "PModItem.h"
#include <QSharedPointer>

class PModItem;

class PController : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int currentlySelectedMod READ currentlySelectedMod NOTIFY modSelected)
    Q_PROPERTY(int modCount READ modCount NOTIFY modAdded)

public:
    explicit PController(QObject *parent = nullptr);
    // ~PController();

    enum Role {
        ModTitleRole = Qt::UserRole + 1,
        ModAuthorRole,
        ModDescriptionRole,
        ModPathRole,
        ModEnabledRole,
        ModCategoryRole,
        ModTagsRole
    };

    int currentlySelectedMod() const;
    int modCount() const;
    void addMod(QSharedPointer<PModItem>);
    void removeMod(int index);
    void selectMod(int index);
    void deselectMod();
    void clearSelection();
    void loadMods();

    PController();
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

signals:
    void modAdded(int index);
    void modRemoved(int index);
    void modSelected(int index);
    void modDeselected();

private:
    QList<QSharedPointer<PModItem>> m_mods_list;
    int m_currentIndex;
};

#endif // PCONTROLLER_H
