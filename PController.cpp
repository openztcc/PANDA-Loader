#include "PController.h"

PController::PController(QObject *parent) : QAbstractListModel(parent), m_currentIndex(-1), m_state(new PState())
{
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
    beginInsertRows(QModelIndex(), m_mods_list.size(), m_mods_list.size());
    m_mods_list.append(mod);
    endInsertRows();

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
    return m_mods_list.size();
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
    beginResetModel();
    // Clear existing mods
    m_mods_list.clear();

    // Load mods from directory
    QStringList ztdList = m_state->getZtdList();

    // Add mods to list
    for (const QString &ztd : ztdList)
    {
        qDebug() << "Loading mod: " << ztd;
        // Load mod from ZTD file
        QSharedPointer<PModItem> mod;
        qDebug() << "Created mod";
        toml::table config = PConfigMgr::getMetaConfig(ztd);
        qDebug() << "Got meta config";
        qDebug() << "Title: " << PConfigMgr::getKeyValue("title", config);
        mod->setmodTitle(PConfigMgr::getKeyValue("title", config));
        qDebug() << "Set title" << PConfigMgr::getKeyValue("title", config);
        mod->setmodAuthor(PConfigMgr::getKeyValue("authors", config));
        mod->setmodDescription(PConfigMgr::getKeyValue("description", config));
        mod->setmodPath(QUrl::fromLocalFile(ztd));
        mod->setmodEnabled(true);
        mod->setmodCategory(PConfigMgr::getKeyValue("category", config));
        mod->setmodTags(PConfigMgr::getKeyValue("tags", config));
        addMod(mod);
    }
    
    // QSharedPointer<PModItem> mod = QSharedPointer<PModItem>::create("Mod 1", "Author 1", "Description 1", QUrl("file:///path/to/mod1"), true, "Category 1", "Tag 1, Tag 2, Tag 3");
    // addMod(mod);

    // QSharedPointer<PModItem> mod2 = QSharedPointer<PModItem>::create("Mod 2", "Author 2", "Description 2", QUrl("file:///path/to/mod2"), true, "Category 2", "Tag 4, Tag 5, Tag 6");
    // addMod(mod2);

    // QSharedPointer<PModItem> mod3 = QSharedPointer<PModItem>::create("Mod 3", "Author 3", "Description 3", QUrl("file:///path/to/mod3"), true, "Category 3", "Tag 7, Tag 8, Tag 9");
    // addMod(mod3);

    endResetModel();
}

void PController::addState(PState *state)
{
    m_state = state;
}
