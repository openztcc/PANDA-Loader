#include "PController.h"

PController::PController(QObject *parent) : QAbstractListModel(parent), m_currentIndex(-1)
{
    loadMods();
}

int PController::currentlySelectedMod() const
{
    return m_currentIndex;
}

int PController::modCount() const
{
    return m_mods_list.size();
}

void PController::addMod(QSharedPointer<PModItem> mod)
{
    m_mods_list.append(mod);
    emit modAdded(m_mods_list.size());
}

void PController::removeMod(int index)
{
    m_mods_list.removeAt(index);
    emit modRemoved(index);
}

void PController::selectMod(int index)
{
    m_currentIndex = index;
    emit modSelected(index);
}

void PController::deselectMod()
{

}

void PController::clearSelection()
{
    m_currentIndex = -1;
    emit modDeselected();
}

int PController::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_mods_list.length();
}

QVariant PController::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && index.row() >= 0 && index.row() < m_mods_list.length())
    {
        QSharedPointer<PModItem> mod = m_mods_list[index.row()];
        switch ((Role) role)
        {
            case ModTitleRole:
                return mod->modTitle();
            case ModAuthorRole:
                return mod->modAuthor();
            case ModDescriptionRole:
                return mod->modDescription();
            case ModPathRole:
                return mod->modPath();
            case ModEnabledRole:
                return mod->modEnabled();
            case ModCategoryRole:
                return mod->modCategory();
            case ModTagsRole:
                return mod->modTags();
        }
    }

    // Return empty if the index is invalid
    return QVariant();
}

// Maps the roles to the data
QHash<int, QByteArray> PController::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[ModTitleRole] = "modTitle";
    roles[ModAuthorRole] = "modAuthor";
    roles[ModDescriptionRole] = "modDescription";
    roles[ModPathRole] = "modPath";
    roles[ModEnabledRole] = "modEnabled";
    roles[ModCategoryRole] = "modCategory";
    roles[ModTagsRole] = "modTags";

    return roles;
}

void PController::loadMods()
{
    // Placeholder for loading mods; dynamic sqlite query later

    PModItem *mod = new PModItem(this);
    mod->setmodTitle("Mod 1");
    mod->setmodAuthor("Author 1");
    mod->setmodDescription("Description 1");
    mod->setmodPath(QUrl("file:///path/to/mod1"));
    mod->setmodEnabled(true);
    mod->setmodCategory("Category 1");
    mod->setmodTags("Tag 1, Tag 2, Tag 3");
    addMod(QSharedPointer<PModItem>::create(mod));

    PModItem *mod2 = new PModItem(this);
    mod2->setmodTitle("Mod 2");
    mod2->setmodAuthor("Author 2");
    mod2->setmodDescription("Description 2");
    mod2->setmodPath(QUrl("file:///path/to/mod2"));
    mod2->setmodEnabled(true);
    mod2->setmodCategory("Category 2");
    mod2->setmodTags("Tag 4, Tag 5, Tag 6");
    addMod(QSharedPointer<PModItem>::create(mod2));
}