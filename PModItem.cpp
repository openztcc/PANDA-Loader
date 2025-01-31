#include "PModItem.h"

// TODO: Implement the PModItem constructor
PModItem::PModItem() {}

PModItem::PModItem(QObject *parent)
{
    m_mod_index = 0;
    m_mod_title = "";
    m_mod_author = "";
    m_mod_description = "";
    m_mod_path = QUrl();
    m_mod_enabled = false;
    m_mod_category = "";
    m_mod_tags = "";

    
}

int PModItem::modIndex() const
{
    return m_mod_index;
}

void PModItem::setmodIndex(int newModIndex)
{
    if (m_mod_index == newModIndex)
        return;

    m_mod_index = newModIndex;
    emit modIndexChanged();
}

QString PModItem::modTitle() const
{
    return m_mod_title;
}

void PModItem::setmodTitle(const QString &newModTitle)
{
    if (m_mod_title == newModTitle)
        return;

    m_mod_title = newModTitle;
    emit modTitleChanged();
}

QString PModItem::modAuthor() const
{
    return m_mod_author;
}

void PModItem::setmodAuthor(const QString &newModAuthor)
{
    if (m_mod_author == newModAuthor)
        return;

    m_mod_author = newModAuthor;
    emit modAuthorChanged();
}

QString PModItem::modDescription() const
{
    return m_mod_description;
}

void PModItem::setmodDescription(const QString &newModDescription)
{
    if (m_mod_description == newModDescription)
        return;

    m_mod_description = newModDescription;
    emit modDescriptionChanged();
}

QUrl PModItem::modPath() const
{
    return m_mod_path;
}

void PModItem::setmodPath(const QUrl &newModPath)
{
    if (m_mod_path == newModPath)
        return;

    m_mod_path = newModPath;
    emit modPathChanged();
}

bool PModItem::modEnabled() const
{
    return m_mod_enabled;
}

void PModItem::setmodEnabled(bool newModEnabled)
{
    if (m_mod_enabled == newModEnabled)
        return;

    m_mod_enabled = newModEnabled;
    emit modEnabledChanged();
}

QString PModItem::modCategory() const
{
    return m_mod_category;
}

void PModItem::setmodCategory(const QString &newModCategory)
{
    if (m_mod_category == newModCategory)
        return;

    m_mod_category = newModCategory;
    emit modCategoryChanged();
}

QString PModItem::modTags() const
{
    return m_mod_tags;
}

void PModItem::setmodTags(const QString &newModTags)
{
    if (m_mod_tags == newModTags)
        return;

    m_mod_tags = newModTags;
    emit modTagsChanged();
}
