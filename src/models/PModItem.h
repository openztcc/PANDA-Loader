#ifndef PMODITEM_H
#define PMODITEM_H

#include <qqml.h>
#include <QString>
#include <QtQml/qqmlregistration.h>

// This class contains all meta data in context of a single mod item.

class PModItem : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int modIndex READ modIndex WRITE setmodIndex NOTIFY modIndexChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY modTitleChanged)
    Q_PROPERTY(QStringList authors READ authors WRITE setAuthors NOTIFY modAuthorChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY modDescriptionChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY modEnabledChanged)
    Q_PROPERTY(QString category READ category WRITE setCategory NOTIFY modCategoryChanged)
    Q_PROPERTY(QStringList tags READ tags WRITE setTags NOTIFY modTagsChanged)
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY modIdChanged)
    Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY modLocationChanged)
    Q_PROPERTY(QString filename READ filename WRITE setFilename NOTIFY modFilenameChanged)
    Q_PROPERTY(QStringList iconpaths READ iconpaths WRITE setIconPaths NOTIFY iconpathsChanged)
    Q_PROPERTY(QString dependencyId READ dependencyId WRITE setDependencyId NOTIFY dependencyIdChanged)
    Q_PROPERTY(QString oglocation READ oglocation WRITE setOGLocation NOTIFY modLocationChanged)
    Q_PROPERTY(QObject* uiComponent READ uiComponent WRITE setUIComponent NOTIFY qmlItemChanged)
    Q_PROPERTY(QString version READ version WRITE setVersion NOTIFY modVersionChanged)

    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY isSelectedChanged)

public:
    explicit PModItem(QObject *parent = nullptr);
    PModItem(const QString &title, const QString &description, const QStringList &authors, 
        const QString &version, bool enabled, const QStringList &tags, const QString &category, 
        const QString &id, const QString &depId, const QString &filename,
        const QString &location, const QStringList &iconpaths, const QString &oglocation, 
        bool selected, QObject *parent = nullptr);
        
    int modIndex() const;
    void setmodIndex(int newModIndex);

    QString title() const;
    void setTitle(const QString &newModTitle);

    QStringList authors() const;
    void setAuthors(const QStringList &newModAuthors);

    QString description() const;
    void setDescription(const QString &newModDescription);

    bool enabled() const;
    void setEnabled(bool newModEnabled);

    QString category() const;
    void setCategory(const QString &newModCategory);

    QStringList tags() const;
    void setTags(const QStringList &newModTags);

    QString id() const;
    void setId(const QString &newModId);

    QString location() const;
    void setLocation(const QString &newModLocation);

    QString filename() const;
    void setFilename(const QString &newModFilename);

    QStringList iconpaths() const;
    void setIconPaths(const QStringList &newModIconPaths);

    QString dependencyId() const;
    void setDependencyId(const QString &newDependencyId);

    QObject* uiComponent() const { return m_ui_component; }
    void setUIComponent(QObject* item);

    QString oglocation() const { return m_oglocation; }
    void setOGLocation(const QString &oglocation) { m_oglocation = oglocation; }

    QString version() const { return m_version; }
    void setVersion(const QString &version) { m_version = version; }

    bool selected() const { return m_selected; }
    void setSelected(bool selected) { 
        if (m_selected != selected) {
            m_selected = selected;
            emit isSelectedChanged();
        }
    }

signals:
    void modIndexChanged();
    void modTitleChanged();
    void modAuthorChanged();
    void modDescriptionChanged();
    void modEnabledChanged();
    void modCategoryChanged();
    void modTagsChanged();
    void modIdChanged();
    void qmlItemChanged();
    void isSelectedChanged();
    void modLocationChanged();
    void modFilenameChanged();
    void iconpathsChanged();
    void dependencyIdChanged();
    void modVersionChanged();

private:
    int m_index;
    QString m_title;
    QStringList m_authors;
    QString m_description;
    bool m_enabled;
    QString m_category;
    QStringList m_tags;
    QString m_id;
    QString m_location;
    QString m_filename;
    QStringList m_iconpaths;
    QString m_dependency_id;
    QObject* m_ui_component = nullptr;
    bool m_selected = false;
    QString m_oglocation;
    QString m_version;
};

#endif // PMODITEM_H

