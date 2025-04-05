#include "PModItem.h"

// TODO: Implement the PModItem constructor
// PModItem::PModItem() {}

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

PModItem::PModItem(const QString &title, const QString &author, const QString &desc, 
    const QUrl &path, bool enabled, const QString &category, const QString &tags, QObject *parent) 
: QObject(parent), 
m_mod_title(title), m_mod_author(author), m_mod_description(desc),
m_mod_path(path), m_mod_enabled(enabled), m_mod_category(category), m_mod_tags(tags)
{
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

QString PModItem::modId() const
{
    return m_mod_id;
}

void PModItem::setmodId(const QString &newModId)
{
    if (m_mod_id == newModId)
        return;

    m_mod_id = newModId;
    emit modIdChanged();
}

QUrl PModItem::modLocation() const
{
    return m_mod_path;
}

void PModItem::setmodLocation(const QUrl &newModLocation)
{
    if (m_mod_path == newModLocation)
        return;

    m_mod_path = newModLocation;
    emit modLocationChanged();
}

QString PModItem::modFilename() const
{
    return m_mod_filename;
}

void PModItem::setmodFilename(const QString &newModFilename)
{
    if (m_mod_filename == newModFilename)
        return;

    m_mod_filename = newModFilename;
    emit modFilenameChanged();
}

QStringList PModItem::modIconPaths() const
{
    return m_mod_iconpaths;
}

void PModItem::setmodIconPaths(const QStringList &newModIconPaths)
{
    if (m_mod_iconpaths == newModIconPaths)
        return;

    m_mod_iconpaths = newModIconPaths;
    emit modIconPathsChanged();
}

QString PModItem::dependencyId() const
{
    return m_dependency_id;
}

void PModItem::setDependencyId(const QString &newDependencyId)
{
    if (m_dependency_id == newDependencyId)
        return;

    m_dependency_id = newDependencyId;
    emit dependencyIdChanged();
}

void PModItem::setQmlItem(QObject* item)
{
    if (m_qmlItem != item) {
        m_qmlItem = item;
        emit qmlItemChanged();
    }
}