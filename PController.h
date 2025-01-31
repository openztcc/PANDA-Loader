#ifndef PCONTROLLER_H
#define PCONTROLLER_H

/* This controller class will manipulate the state of the application. Mainly, it
concerns itself with the mods list and operations over other classes from the UI. */

#include <QObject>
#include <QAbstractListModel>


class PController : public QAbstractListModel
{
    Q_OBJECT

    Q_P
public:
    explicit PController(QObject *parent = nullptr);

    enum {
        ModTitleRole = Qt::UserRole + 1,
        ModAuthorRole,
        ModDescriptionRole,
        ModPathRole,
        ModEnabledRole,
        ModCategoryRole,
        ModTagsRole
    };

    int currentlySelectedMod() const;
    int modCount() const;
    void addMod(const std::unordered_map<QString, PModItem> &modName);
    void removeMod(int index);
    void selectMod(int index);
    void deselectMod();
    void clearSlection();

    PController();
    ~PController();
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

signals:
    void modAdded(int index);
    void modRemoved(int index);
    void modSelected(int index);
    void modDeselected();

private:
    std::unordered_map<QString, PModItem> m_mods;
    int m_currentIndex;
};

#endif // PCONTROLLER_H
