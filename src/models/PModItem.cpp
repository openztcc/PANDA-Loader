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
                   bool selected, bool listed, QObject *parent)

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
    m_selected(selected),
    m_listed(listed)
{
    m_index = 0;
    m_ui_component = nullptr;
    m_selected = false;
    m_listed = true;
}

PModItem::PModItem(QObject *parent, const QSqlQuery &query) : QObject(parent)
{
    m_index = 0;
    setTitle(query.value("title").toString());
    setAuthors(query.value("authors").toString().split(", "));
    setDescription(query.value("description").toString());
    setEnabled(query.value("enabled").toBool());
    setCategory(query.value("category").toString());
    setTags(query.value("tags").toString().split(", "));
    setId(query.value("mod_id").toString());
    setFilename(query.value("filename").toString());
    setIconPaths(query.value("iconpaths").toString().split(", "));
    setDependencyId(query.value("dep_id").toString());
    setLocation(query.value("location").toString());
    setOGLocation(query.value("oglocation").toString());
    setSelected(query.value("is_selected").toBool());
    setVersion(query.value("version").toString());
    setListed(query.value("listed").toBool());
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
    return m_title.isEmpty() ? "Unknown" : m_title;
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
    return m_authors.isEmpty() ? QStringList{"Unknown"} : m_authors;
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
    return m_description.isEmpty() ? "No description available" : m_description;
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
    return m_enabled ? true : false;
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
    return m_category.isEmpty() ? "Unknown" : m_category;
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
    return m_tags.isEmpty() ? QStringList() : m_tags;
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
    return m_id.isEmpty() ? "Unknown" : m_id;
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
    return m_location.isEmpty() ? "Unknown" : m_location;
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
    return m_filename.isEmpty() ? "Unknown" : m_filename;
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
    return m_iconpaths.isEmpty() ? QStringList() : m_iconpaths;
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
    return m_dependency_id.isEmpty() ? "Unknown" : m_dependency_id;
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

bool PModItem::listed() const
{
    return m_listed ? true : false;
}

QVariant PModItem::getData(int role) const
{
    switch ((Role) role)
    {
        case ModTitleRole:
            return title();
        case ModAuthorRole:
            return authors();
        case ModDescriptionRole:
            return description();
        case ModEnabledRole:
            return enabled();
        case ModCategoryRole:
            return category();
        case ModTagsRole:
            return tags();
        case ModIdRole:
            return id();
        case ModFilenameRole:
            return filename();
        case ModDependencyIdRole:
            return dependencyId();
        case ModLocationRole:
            return location();
        case ModIconPathsRole:
            return iconpaths();
        case ModOgLocationRole:
            return oglocation();
        case ModSelectedRole:
            return selected();
        case ModVersionRole:
            return version();
        case ModListedRole:
            return listed();
        case ModObjectRole:
            qDebug() << "Returning mod object: " << title();
            return QVariant::fromValue(this); // return a whole mod object
    }
}

QHash<int, QByteArray> PModItem::roleNames()
{
    QHash<int, QByteArray> roles;

    roles[ModTitleRole] = "title";
    roles[ModAuthorRole] = "authors";
    roles[ModDescriptionRole] = "description";
    roles[ModEnabledRole] = "enabled";
    roles[ModCategoryRole] = "category";
    roles[ModTagsRole] = "tags";
    roles[ModIdRole] = "id";
    roles[ModFilenameRole] = "filename";
    roles[ModDependencyIdRole] = "depId";
    roles[ModLocationRole] = "location";
    roles[ModObjectRole] = "instance"; // return a whole mod object
    roles[ModIconPathsRole] = "iconpaths";
    roles[ModOgLocationRole] = "oglocation";
    roles[ModSelectedRole] = "selected";
    roles[ModVersionRole] = "version";
    roles[ModListedRole] = "listed";

    return roles;
}

