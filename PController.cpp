#include "PController.h"

PController::PController(QObject *parent)
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
    m_mods_list.append(QSharedPointer<PModItem>(mod));
    emit modAdded(m_mods_list.size());
}

void PController::removeMod(int index)
{
    m_mods_list.removeAt(index);
    emit modRemoved(index);
}

void PController::selectMod(int index)
{

}

void PController::deselectMod()
{

}

void PController::clearSlection()
{

}

int PController::rowCount(const QModelIndex &parent) const
{

}

QVariant PController::data(const QModelIndex &index, int role) const
{

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
