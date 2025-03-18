#ifndef PMODITEM_H
#define PMODITEM_H

#include <qqml.h>
#include <QUrl>
#include <QtQml/qqmlregistration.h>

// This class contains all meta data in context of a single mod item.

class PModItem : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int modIndex READ modIndex WRITE setmodIndex NOTIFY modIndexChanged)
    Q_PROPERTY(QString modTitle READ modTitle WRITE setmodTitle NOTIFY modTitleChanged)
    Q_PROPERTY(QString modAuthor READ modAuthor WRITE setmodAuthor NOTIFY modAuthorChanged)
    Q_PROPERTY(QString modDescription READ modDescription WRITE setmodDescription NOTIFY modDescriptionChanged)
    Q_PROPERTY(QUrl modPath READ modPath WRITE setmodPath NOTIFY modPathChanged)
    Q_PROPERTY(bool modEnabled READ modEnabled WRITE setmodEnabled NOTIFY modEnabledChanged)
    Q_PROPERTY(QString modCategory READ modCategory WRITE setmodCategory NOTIFY modCategoryChanged)
    Q_PROPERTY(QString modTags READ modTags WRITE setmodTags NOTIFY modTagsChanged)
public:
    explicit PModItem(QObject *parent = nullptr);
    PModItem(const QString &title, const QString &author, const QString &desc, 
        const QUrl &path, bool enabled, const QString &category, const QString &tags, QObject *parent = nullptr);
        
    int modIndex() const;
    void setmodIndex(int newModIndex);

    QString modTitle() const;
    void setmodTitle(const QString &newModTitle);

    QString modAuthor() const;
    void setmodAuthor(const QString &newModAuthor);

    QString modDescription() const;
    void setmodDescription(const QString &newModDescription);

    QUrl modPath() const;
    void setmodPath(const QUrl &newModPath);

    bool modEnabled() const;
    void setmodEnabled(bool newModEnabled);

    QString modCategory() const;
    void setmodCategory(const QString &newModCategory);

    QString modTags() const;
    void setmodTags(const QString &newModTags);

signals:
    void modIndexChanged();
    void modTitleChanged();
    void modAuthorChanged();
    void modDescriptionChanged();
    void modPathChanged();
    void modEnabledChanged();
    void modCategoryChanged();
    void modTagsChanged();

private:
    int m_mod_index;
    QString m_mod_title;
    QString m_mod_author;
    QString m_mod_description;
    QUrl m_mod_path;
    bool m_mod_enabled;
    QString m_mod_category;
    QString m_mod_tags;
};

#endif // PMODITEM_H
