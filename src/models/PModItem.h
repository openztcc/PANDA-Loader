#ifndef PMODITEM_H
#define PMODITEM_H

#include <qqml.h>
#include <QString>
#include <QtQml/qqmlregistration.h>
#include <QSqlQuery>

// This class contains all meta data in context of a single mod item in a QAbstractListModel

class PModItem : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    // Mod Properties
    Q_PROPERTY(int modIndex READ modIndex WRITE setmodIndex NOTIFY modIndexChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY modTitleChanged)
    Q_PROPERTY(QStringList authors READ authors WRITE setAuthors NOTIFY modAuthorChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY modDescriptionChanged)
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY modIdChanged)
    Q_PROPERTY(QString version READ version WRITE setVersion NOTIFY modVersionChanged)
    Q_PROPERTY(QString link READ link WRITE setLink NOTIFY modLinkChanged)

    // Mod flags
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY modEnabledChanged)
    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY isSelectedChanged)
    Q_PROPERTY(bool listed READ listed WRITE setListed NOTIFY modListedChanged)
    Q_PROPERTY(bool isCollection READ isCollection WRITE setIsCollection NOTIFY modIsCollectionChanged)

    // Categorization properties
    Q_PROPERTY(QString category READ category WRITE setCategory NOTIFY modCategoryChanged)
    Q_PROPERTY(QStringList tags READ tags WRITE setTags NOTIFY modTagsChanged)

    // File data properties
    Q_PROPERTY(QString currentLocation READ currentLocation WRITE setCurrentLocation NOTIFY modCurrentLocationChanged)
    Q_PROPERTY(QString originalLocation READ originalLocation WRITE setOriginalLocation NOTIFY modOriginalLocationChanged)
    Q_PROPERTY(QString disabledLocation READ disabledLocation WRITE setDisabledLocation NOTIFY modDisabledLocationChanged)
    Q_PROPERTY(QString filename READ filename WRITE setFilename NOTIFY modFilenameChanged)
    Q_PROPERTY(QString fileSize READ fileSize WRITE setFileSize NOTIFY modFileSizeChanged)
    Q_PROPERTY(QString fileDate READ fileDate WRITE setFileDate NOTIFY modFileDateChanged)

    // Graphics properties
    Q_PROPERTY(QStringList iconpaths READ iconpaths WRITE setIconPaths NOTIFY iconpathsChanged)

    // External properties
    Q_PROPERTY(QString dependencyId READ dependencyId WRITE setDependencyId NOTIFY dependencyIdChanged)
    Q_PROPERTY(QString collectionId READ collectionId WRITE setCollectionId NOTIFY collectionIdChanged)

    // Instance
    Q_PROPERTY(QObject* uiComponent READ uiComponent WRITE setUIComponent NOTIFY qmlItemChanged)


public:
    enum Role {
        // Mod properties
        ModTitleRole = Qt::UserRole + 1,
        ModAuthorRole,
        ModDescriptionRole,
        ModIdRole,
        ModVersionRole,
        ModLinkRole,
        
        // Categorization properties
        ModCategoryRole,
        ModTagsRole,

        // Mod flags
        ModEnabledRole,
        ModListedRole,
        ModSelectedRole,
        ModIsCollectionRole,

        // Graphics properties
        ModIconPathsRole,

        // File data properties
        ModFilenameRole,
        ModCurrentLocationRole,
        ModOriginalLocationRole,
        ModDisabledLocationRole,
        ModFileSizeRole,
        ModFileDateRole,

        // Instance
        ModObjectRole,

        // External properties
        ModDependencyIdRole,
        ModCollectionIdRole
    };

    explicit PModItem(QObject *parent = nullptr);
    PModItem(
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
        QObject *parent = nullptr
    );
    PModItem(QObject *parent, const QSqlQuery &query);
        
    int modIndex() const;
    void setmodIndex(int newModIndex);

    // Mod properties

    QString title() const;
    void setTitle(const QString &newModTitle);
    QStringList authors() const;
    void setAuthors(const QStringList &newModAuthors);
    QString description() const;
    void setDescription(const QString &newModDescription);
    QString id() const;
    void setId(const QString &newModId);
    QString version() const;
    void setVersion(const QString &version);
    QString link() const;
    void setLink(const QString &link);

    // Mod flags
    bool enabled() const;
    void setEnabled(bool newModEnabled);
    bool selected() const;
    void setSelected(bool selected);
    bool listed() const;
    void setListed(bool listed);
    bool isCollection() const;
    void setIsCollection(bool isCollection);

    // Categorization properties
    QString category() const;
    void setCategory(const QString &newModCategory);
    QStringList tags() const;
    void setTags(const QStringList &newModTags);

    // Graphics properties
    QStringList iconpaths() const;
    void setIconPaths(const QStringList &newModIconPaths);

    // File data properties
    QString currentLocation() const;
    void setCurrentLocation(const QString &newModLocation);
    QString filename() const;
    void setFilename(const QString &newModFilename);
    QString originalLocation() const;
    void setOriginalLocation(const QString &oglocation);
    QString disabledLocation() const;
    void setDisabledLocation(const QString &disabledLocation);
    QString fileSize() const;
    void setFileSize(const QString &fileSize);
    QString fileDate() const;
    void setFileDate(const QString &fileDate);

    // External properties
    QString dependencyId() const;
    void setDependencyId(const QString &newDependencyId);
    QString collectionId() const;
    void setCollectionId(const QString &collectionId);

    // Instance
    QObject* uiComponent() const { return m_ui_component; }
    void setUIComponent(QObject* item);

    QVariant getData(int role) const;
    static QHash<int, QByteArray> roleNames();

signals:
    // Mod properties
    void modIndexChanged();
    void modTitleChanged();
    void modAuthorChanged();
    void modDescriptionChanged();
    void modIdChanged();
    void modVersionChanged();
    void modLinkChanged();

    // Mod flags
    void modEnabledChanged();
    void isSelectedChanged();
    void modListedChanged();
    void modIsCollectionChanged();

    // Categorization properties
    void modCategoryChanged();
    void modTagsChanged();

    // Instance
    void qmlItemChanged();

    // File data properties
    void modCurrentLocationChanged();
    void modOriginalLocationChanged();
    void modDisabledLocationChanged();
    void modFilenameChanged();
    void modFileSizeChanged();
    void modFileDateChanged();

    // Graphics properties
    void iconpathsChanged();

    // External properties
    void dependencyIdChanged();
    void collectionIdChanged();

private:
    // Mod properties
    int m_index;
    QString m_title;
    QStringList m_authors;
    QString m_description;
    QString m_id;
    QString m_version;
    QString m_link;

    // Mod flags
    bool m_enabled;
    bool m_selected = false;
    bool m_listed;
    bool m_isCollection = false;

    // Categorization properties
    QString m_category;
    QStringList m_tags;
    
    // File data properties
    QString m_current_location;
    QString m_filename;
    QString m_original_location;
    QString m_disabled_location;
    QString m_fileSize;
    QString m_fileDate;

    // Graphics properties
    QStringList m_iconpaths;

    // External properties
    QString m_dependency_id;
    QString m_collection_id;

    // Instance
    QObject* m_ui_component = nullptr;
};

#endif // PMODITEM_H

