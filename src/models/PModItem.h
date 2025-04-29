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

    // Mod flags
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY modEnabledChanged)
    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY isSelectedChanged)

    // Categorization properties
    Q_PROPERTY(QString category READ category WRITE setCategory NOTIFY modCategoryChanged)
    Q_PROPERTY(QStringList tags READ tags WRITE setTags NOTIFY modTagsChanged)

    // File data properties
    Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY modLocationChanged)
    Q_PROPERTY(QString filename READ filename WRITE setFilename NOTIFY modFilenameChanged)
    Q_PROPERTY(QString oglocation READ oglocation WRITE setOGLocation NOTIFY modLocationChanged)

    // Graphics properties
    Q_PROPERTY(QStringList iconpaths READ iconpaths WRITE setIconPaths NOTIFY iconpathsChanged)

    // External properties
    Q_PROPERTY(QString dependencyId READ dependencyId WRITE setDependencyId NOTIFY dependencyIdChanged)

    // Instance
    Q_PROPERTY(QObject* uiComponent READ uiComponent WRITE setUIComponent NOTIFY qmlItemChanged)


public:
    enum Role {
        // Mod properties
        ModTitleRole = Qt::UserRole + 1,
        ModAuthorRole,
        ModDescriptionRole,
        ModIdRole,
        ModVersionRole
        
        // Categorization properties
        ModCategoryRole,
        ModTagsRole,

        // Mod flags
        ModEnabledRole,
        ModListedRole,
        ModSelectedRole,

        // Graphics properties
        ModIconPathsRole,

        // File data properties
        ModFilenameRole,
        ModCurrentLocationRole,
        ModOriginalLocationRole,
        ModDisabledLocationRole,

        // Instance
        ModObjectRole,

        // External properties
        ModDependencyIdRole,
    };

    explicit PModItem(QObject *parent = nullptr);
    PModItem(const QString &title, const QString &description, const QStringList &authors, 
        const QString &version, bool enabled, const QStringList &tags, const QString &category, 
        const QString &id, const QString &depId, const QString &filename,
        const QString &location, const QStringList &iconpaths, const QString &oglocation, 
        bool selected, QObject *parent = nullptr);
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
    QString version() const { return m_version; }
    void setVersion(const QString &version) { m_version = version; }

    // Mod flags
    bool enabled() const;
    void setEnabled(bool newModEnabled);
    bool selected() const { return m_selected; }
    void setSelected(bool selected) { 
        if (m_selected != selected) {
            m_selected = selected;
            emit isSelectedChanged();
        }
    }
    bool listed() const { return m_listed; }
    void setListed(bool listed) { m_listed = listed; }

    // Categorization properties
    QString category() const;
    void setCategory(const QString &newModCategory);
    QStringList tags() const;
    void setTags(const QStringList &newModTags);

    // Graphics properties
    QStringList iconpaths() const;
    void setIconPaths(const QStringList &newModIconPaths);

    // File data properties
    QString location() const;
    void setLocation(const QString &newModLocation);
    QString filename() const;
    void setFilename(const QString &newModFilename);
    QString oglocation() const { return m_oglocation; }
    void setOGLocation(const QString &oglocation) { m_oglocation = oglocation; }

    // External properties
    QString dependencyId() const;
    void setDependencyId(const QString &newDependencyId);

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

    // Mod flags
    void modEnabledChanged();
    void isSelectedChanged();
    void modListedChanged();

    // Categorization properties
    void modCategoryChanged();
    void modTagsChanged();

    // Instance
    void qmlItemChanged();

    // File data properties
    void modLocationChanged();
    void modFilenameChanged();

    // Graphics properties
    void iconpathsChanged();

    // External properties
    void dependencyIdChanged();

private:
    // Mod properties
    int m_index;
    QString m_title;
    QStringList m_authors;
    QString m_description;
    QString m_id;
    QString m_version;

    // Mod flags
    bool m_enabled;
    bool m_selected = false;
    bool m_listed;

    // Categorization properties
    QString m_category;
    QStringList m_tags;
    
    // File data properties
    QString m_location;
    QString m_filename;
    QString m_oglocation;

    // Graphics properties
    QStringList m_iconpaths;

    // External properties
    QString m_dependency_id;

    // Instance
    QObject* m_ui_component = nullptr;
};

#endif // PMODITEM_H

