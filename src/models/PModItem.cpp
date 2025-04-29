#include "PModItem.h"

// TODO: Implement the PModItem constructor
// PModItem::PModItem() {}

PModItem::PModItem(QObject *parent)
{
    // Mod Properties
    m_index = 0;
    m_title = "";
    m_authors = {""};
    m_description = "";
    m_id = "";
    m_version = "";
    m_link = "";

    // Mod flags
    m_enabled = false;
    m_selected = false;
    m_listed = true;
    m_isCollection = false;

    // Categorization properties
    m_category = "";
    m_tags = {""};

    // Graphics properties
    m_iconpaths = QStringList();

    // File data properties
    m_current_location = "";
    m_filename = "";
    m_original_location = "";
    m_disabled_location = "";
    m_fileSize = "";
    m_fileDate = "";

    // External properties
    m_dependency_id = "";
    m_collection_id = "";

    // Instance
    m_ui_component = nullptr;
}

PModItem::PModItem(
        // Mod properties
        const QString &title, 
        const QString &description, 
        const QStringList &authors, 
        const QString &version, 
        const QString &id, 
        const QString &link,

        // Mod flags
        bool enabled, 
        bool selected, 
        bool listed, 
        bool isCollection,

        // Categorization properties
        const QStringList &tags, 
        const QString &category, 

        // External properties
        const QString &depId, 
        const QString &collectionId,

        // File data properties
        const QString &filename,
        const QString &currentLocation, 
        const QString &originalLocation, 
        const QString &disabledLocation, 
        const QString &fileSize,
        const QString &fileDate,

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
    m_version(version),
    m_link(link),

    // Mod flags
    m_enabled(enabled),
    m_selected(selected),
    m_listed(listed),
    m_isCollection(isCollection),

    // Categorization properties
    m_category(category),
    m_tags(tags),

    // File data properties
    m_current_location(currentLocation),
    m_filename(filename),
    m_original_location(originalLocation),
    m_disabled_location(disabledLocation),
    m_fileSize(fileSize),
    m_fileDate(fileDate),

    // Graphics properties
    m_iconpaths(iconpaths),

    // External properties
    m_dependency_id(depId),
    m_collection_id(collectionId),
    m_ui_component(nullptr)
{
    m_index = 0;
    m_ui_component = nullptr;
    m_selected = false;
    m_listed = true;
}

PModItem::PModItem(QObject *parent, const QSqlQuery &query) : QObject(parent)
{
    // Mod properties
    m_index = 0;
    setTitle(query.value("title").toString());
    setAuthors(query.value("authors").toString().split(", "));
    setDescription(query.value("description").toString());
    setId(query.value("mod_id").toString());
    setVersion(query.value("version").toString());
    setLink(query.value("link").toString());

    // Mod flags
    setEnabled(query.value("enabled").toBool());
    setIsCollection(query.value("is_collection").toBool());
    setSelected(query.value("is_selected").toBool());
    setListed(query.value("listed").toBool());

    // External properties
    setDependencyId(query.value("dep_id").toString());
    setCollectionId(query.value("collection_id").toString());

    // Categorization properties
    setTags(query.value("tags").toString().split(", "));
    setCategory(query.value("category").toString());

    // File data properties
    setFilename(query.value("filename").toString());
    setCurrentLocation(query.value("current_location").toString());
    setOriginalLocation(query.value("original_location").toString());
    setDisabledLocation(query.value("disabled_location").toString());
    setFileSize(query.value("file_size").toString());
    setFileDate(query.value("fil_date").toString());

    // Graphics properties
    setIconPaths(query.value("icon_paths").toString().split(", "));

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

QString PModItem::version() const
{
    return m_version.isEmpty() ? "Unknown" : m_version;
}

void PModItem::setVersion(const QString &newModVersion)
{
    if (m_version == newModVersion)
        return;

    m_version = newModVersion;
    emit modVersionChanged();
}

QString PModItem::link() const
{
    return m_link.isEmpty() ? "Unknown" : m_link;
}

void PModItem::setLink(const QString &newModLink)
{
    if (m_link == newModLink)
        return;

    m_link = newModLink;
    emit modLinkChanged();
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

bool PModItem::listed() const
{
    return m_listed ? true : false;
}

void PModItem::setListed(bool listed)
{
    if (m_listed == listed)
        return;

    m_listed = listed;
    emit modListedChanged();
}

bool PModItem::selected() const
{
    return m_selected ? true : false;
}

void PModItem::setSelected(bool selected)
{
    if (m_selected == selected)
        return;

    m_selected = selected;
    emit isSelectedChanged();
}

bool PModItem::isCollection() const
{
    return m_isCollection ? true : false;
}

void PModItem::setIsCollection(bool isCollection)
{
    if (m_isCollection == isCollection)
        return;

    m_isCollection = isCollection;
    emit modIsCollectionChanged();
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

QString PModItem::originalLocation() const
{
    return m_original_location.isEmpty() ? "Unknown" : m_original_location;
}

void PModItem::setOriginalLocation(const QString &oglocation)
{
    if (m_original_location == oglocation)
        return;

    m_original_location = oglocation;
    emit modOriginalLocationChanged();
}

QString PModItem::disabledLocation() const
{
    return m_disabled_location.isEmpty() ? "Unknown" : m_disabled_location;
}

void PModItem::setDisabledLocation(const QString &disabledLocation)
{
    if (m_disabled_location == disabledLocation)
        return;

    m_disabled_location = disabledLocation;
    emit modDisabledLocationChanged();
}

QString PModItem::fileSize() const
{
    return m_fileSize.isEmpty() ? "Unknown" : m_fileSize;
}

void PModItem::setFileSize(const QString &fileSize)
{
    if (m_fileSize == fileSize)
        return;

    m_fileSize = fileSize;
    emit modFileSizeChanged();
}

QString PModItem::fileDate() const
{
    return m_fileDate.isEmpty() ? "Unknown" : m_fileDate;
}

void PModItem::setFileDate(const QString &fileDate)
{
    if (m_fileDate == fileDate)
        return;

    m_fileDate = fileDate;
    emit modFileDateChanged();
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

QString PModItem::collectionId() const
{
    return m_collection_id.isEmpty() ? "Unknown" : m_collection_id;
}

void PModItem::setCollectionId(const QString &newCollectionId)
{
    if (m_collection_id == newCollectionId)
        return;

    m_collection_id = newCollectionId;
    emit collectionIdChanged();
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
    roles[ModLinkRole] = "link";

    // Mod flags
    roles[ModEnabledRole] = "enabled";
    roles[ModSelectedRole] = "selected";
    roles[ModListedRole] = "listed";
    roles[ModIsCollectionRole] = "isCollection";

    // Categorization properties
    roles[ModCategoryRole] = "category";
    roles[ModTagsRole] = "tags";

    // File data properties
    roles[ModFilenameRole] = "filename";
    roles[ModCurrentLocationRole] = "currentLocation";
    roles[ModOriginalLocationRole] = "originalLocation";
    roles[ModDisabledLocationRole] = "disabledLocation";
    roles[ModFileSizeRole] = "fileSize";
    roles[ModFileDateRole] = "fileDate";

    // External properties
    roles[ModDependencyIdRole] = "depId";
    roles[ModCollectionIdRole] = "collectionId";

    // Instance
    roles[ModObjectRole] = "instance"; // return a whole mod object

    // Graphics properties
    roles[ModIconPathsRole] = "iconpaths";

    return roles;
}

