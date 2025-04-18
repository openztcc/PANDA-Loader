#include "PModItem.h"

// TODO: Implement the PModItem constructor
// PModItem::PModItem() {}

PModItem::PModItem(QObject *parent)
{
    m_index = 0;
    m_title = "";
    m_authors = {""};
    m_description = "";
    m_enabled = false;
    m_category = "";
    m_tags = {""};
    m_iconpaths = QStringList();
    m_location = "";
    m_filename = "";
    m_dependency_id = "";
}

PModItem::PModItem(const QString &title, const QString &description, const QStringList &authors,
                   const QString &version, bool enabled, const QStringList &tags, const QString &category,
                   const QString &id, const QString &depId, const QString &filename,
                   const QString &location, const QStringList &iconpaths, const QString &oglocation,
                   bool selected, QObject *parent)

: QObject(parent), 
    m_title(title),
    m_authors(authors),
    m_description(description),
    m_enabled(enabled),
    m_category(category),
    m_tags(tags),
    m_id(id),
    m_location(location),
    m_filename(filename),
    m_iconpaths(iconpaths),
    m_dependency_id(depId),
    m_oglocation(oglocation),
    m_selected(selected)
{
    m_index = 0;
    m_ui_component = nullptr;
    m_selected = false;
}

int PModItem::modIndex() const
{
    return m_index;
}

void PModItem::setmodIndex(int newModIndex)
{
    if (m_index == newModIndex)
        return;

    m_index = newModIndex;
    emit modIndexChanged();
}

QString PModItem::title() const
{
    return m_title;
}

void PModItem::setTitle(const QString &newModTitle)
{
    if (m_title == newModTitle)
        return;

    m_title = newModTitle;
    emit modTitleChanged();
}

QStringList PModItem::authors() const
{
    return m_authors;
}

void PModItem::setAuthors(const QStringList &newModAuthor)
{
    if (m_authors == newModAuthor)
        return;

    m_authors = newModAuthor;
    emit modAuthorChanged();
}

QString PModItem::description() const
{
    return m_description;
}

void PModItem::setDescription(const QString &newModDescription)
{
    if (m_description == newModDescription)
        return;

    m_description = newModDescription;
    emit modDescriptionChanged();
}

bool PModItem::enabled() const
{
    return m_enabled;
}

void PModItem::setEnabled(bool newModEnabled)
{
    if (m_enabled == newModEnabled)
        return;

    m_enabled = newModEnabled;
    emit modEnabledChanged();
}

QString PModItem::category() const
{
    return m_category;
}

void PModItem::setCategory(const QString &newModCategory)
{
    if (m_category == newModCategory)
        return;

    m_category = newModCategory;
    emit modCategoryChanged();
}

QStringList PModItem::tags() const
{
    return m_tags;
}

void PModItem::setTags(const QStringList &newModTags)
{
    if (m_tags == newModTags)
        return;

    m_tags = newModTags;
    emit modTagsChanged();
}

QString PModItem::id() const
{
    return m_id;
}

void PModItem::setId(const QString &newModId)
{
    if (m_id == newModId)
        return;

    m_id = newModId;
    emit modIdChanged();
}

QString PModItem::location() const
{
    return m_location;
}

void PModItem::setLocation(const QString &newModLocation)
{
    if (m_location == newModLocation)
        return;

    m_location = newModLocation;
    emit modLocationChanged();
}

QString PModItem::filename() const
{
    return m_filename;
}

void PModItem::setFilename(const QString &newModFilename)
{
    if (m_filename == newModFilename)
        return;

    m_filename = newModFilename;
    emit modFilenameChanged();
}

QStringList PModItem::iconpaths() const
{
    return m_iconpaths;
}

void PModItem::setIconPaths(const QStringList &newModIconPaths)
{
    if (m_iconpaths == newModIconPaths)
        return;

    m_iconpaths = newModIconPaths;
    emit iconpathsChanged();
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

void PModItem::setUIComponent(QObject* item)
{
    if (m_ui_component != item) {
        m_ui_component = item;
        emit qmlItemChanged();
    }
}
