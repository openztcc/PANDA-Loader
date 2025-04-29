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
    m_current_location = "";
    m_filename = "";
    m_dependency_id = "";
}

PModItem::PModItem(
        // Mod properties
        const QString &title, 
        const QString &description, 
        const QStringList &authors, 
        const QString &version, 
        const QString &id, 

        // Mod flags
        bool enabled, 
        bool selected, 
        bool listed, 

        // Categorization properties
        const QStringList &tags, 
        const QString &category, 

        // External properties
        const QString &depId, 

        // File data properties
        const QString &filename,
        const QString &currentLocation, 
        const QString &originalLocation, 
        const QString &disabledLocation, 

        // Graphics properties
        const QStringList &iconpaths, 

        // Instance
        QObject *parent
)
: QObject(parent), 

    // Mod properties
    m_title(title),
    m_authors(authors),
    m_description(description),
    m_id(id),

    // Mod flags
    m_enabled(enabled),
    m_selected(selected),
    m_listed(listed),

    // Categorization properties
    m_category(category),
    m_tags(tags),

    // File data properties
    m_current_location(currentLocation),
    m_filename(filename),
    m_original_location(originalLocation),
    m_disabled_location(disabledLocation),

    // Graphics properties
    m_iconpaths(iconpaths),

    // External properties
    m_dependency_id(depId)
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
    setCurrentLocation(query.value("currentLocation").toString());
    setOriginalLocation(query.value("originalLocation").toString());
    setSelected(query.value("is_selected").toBool());
    setVersion(query.value("version").toString());
    setListed(query.value("listed").toBool());
}

// ----------------------------------------------- Mod properties

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

// ----------------------------------------------- Mod flags

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

// ----------------------------------------------- Mod categorization

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

// ----------------------------------------------- File data properties

QString PModItem::currentLocation() const
{
    return m_current_location.isEmpty() ? "Unknown" : m_current_location;
}

void PModItem::setCurrentLocation(const QString &newModLocation)
{
    if (m_current_location == newModLocation)
        return;

    m_current_location = newModLocation;
    emit modCurrentLocationChanged();
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

// ------------------------------------------------ Graphics properties

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

// ------------------------------------------------ External properties

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

// ------------------------------------------------ Instance

void PModItem::setUIComponent(QObject* item)
{
    if (m_ui_component != item) {
        m_ui_component = item;
        emit qmlItemChanged();
    }
}

QVariant PModItem::getData(int role) const
{
    switch ((Role) role)
    {
        // Mod properties
        case ModTitleRole:
            return title();
        case ModAuthorRole:
            return authors();
        case ModDescriptionRole:
            return description();
        case ModIdRole:
            return id();
        case ModVersionRole:
            return version();

        // Mod flags
        case ModEnabledRole:
            return enabled();
        case ModListedRole:
            return listed();
        case ModSelectedRole:
            return selected();
        
        // Categorization properties
        case ModCategoryRole:
            return category();
        case ModTagsRole:
            return tags();

        // File data properties
        case ModFilenameRole:
            return filename();
        case ModCurrentLocationRole:
            return currentLocation();
        case ModOriginalLocationRole:
            return originalLocation();
        case ModDisabledLocationRole:
            return disabledLocation();

        // External properties
        case ModDependencyIdRole:
            return dependencyId();
        
        // Graphics properties
        case ModIconPathsRole:
            return iconpaths();

        // Instance
        case ModObjectRole:
            qDebug() << "Returning mod object: " << title();
            return QVariant::fromValue(this); // return a whole mod object
    }
}

QHash<int, QByteArray> PModItem::roleNames()
{
    QHash<int, QByteArray> roles;

    // Mod properties
    roles[ModTitleRole] = "title";
    roles[ModAuthorRole] = "authors";
    roles[ModDescriptionRole] = "description";
    roles[ModIdRole] = "id";
    roles[ModVersionRole] = "version";

    // Mod flags
    roles[ModEnabledRole] = "enabled";
    roles[ModSelectedRole] = "selected";
    roles[ModListedRole] = "listed";

    // Categorization properties
    roles[ModCategoryRole] = "category";
    roles[ModTagsRole] = "tags";

    // File data properties
    roles[ModFilenameRole] = "filename";
    roles[ModCurrentLocationRole] = "currentLocation";
    roles[ModOriginalLocationRole] = "originalLocation";
    roles[ModDisabledLocationRole] = "disabledLocation";

    // External properties
    roles[ModDependencyIdRole] = "depId";

    // Instance
    roles[ModObjectRole] = "instance"; // return a whole mod object

    // Graphics properties
    roles[ModIconPathsRole] = "iconpaths";

    return roles;
}

