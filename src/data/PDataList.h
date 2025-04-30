#ifndef PDataList_H
#define PDataList_H

// This is a generic implementation of a QAbstractListModel class that is meant to be 
// reused for any data type. Ideally any type that uses it must define both the getData() and
// roleNames() methods. Additionally, a QAbstractListModel-compatible rolenames enum struct
// must be defined in the class that uses this template.

// Qt
#include <QObject>
#include <QAbstractListModel>

template <typename T>
class PDataList : public QAbstractListModel
{
public:
    PDataList(QObject *parent) : QAbstractListModel(parent) {}

    // size of the list
    int rowCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent);
        return m_list.size();
    }

    // return the data for the given index and role
    QVariant data(const QModelIndex &index, int role) const
    {
        qDebug () << "Fetching data for index: " << index.row() << ", role: " << role;
        if (index.isValid() && index.row() >= 0 && index.row() < m_list.length())
        {
            T item = m_list[index.row()];
            item->getData(role);
            return item->getData(role);
        }

        // Return empty if the index is invalid
        qDebug() << "Invalid index: " << index.row() << ", returning empty QVariant";
        return QVariant();
    }

    // return the role names for the model
    QHash<int, QByteArray> roleNames()
    {
        return T::roleNames();
    }

    // add item to the list
    void addItem(T item)
    {
        beginInsertRows(QModelIndex(), m_list.size(), m_list.size());
        m_list.append(item);
        endInsertRows();
    }

    // remove item from the list
    void removeItem(int index)
    {
        beginRemoveRows(QModelIndex(), index, index);
        m_list.removeAt(index);
        endRemoveRows();
    }

    // remove multiple items from the list
    void removeItems(QVector<T> items)
    {
        beginResetModel();
        for (const auto& item : items) {
            m_list.removeOne(item);
        }
        endResetModel();
    }

    // replace item in the list
    void replaceItem(int row, T item)
    {
        if (row >= 0 && row < m_list.size()) {
            beginResetModel();
            m_list[row] = item;
            endResetModel();
        }
    }

    void replaceList(QVector<T> list)
    {
        beginResetModel();
        m_list = list;
        endResetModel();
    }

    // clear the list
    void clearList()
    {
        beginResetModel();
        m_list.clear();
        endResetModel();
    }

    int size() const
    {
        return m_list.size();
    }

    int indexOf(T item) const
    {
        return m_list.indexOf(item);
    }

    T getItem(int index) const
    {
        if (index >= 0 && index < m_list.size()) {
            return m_list[index];
        }
        return nullptr; // Return null if index is out of bounds
    }

private:
    QVector<T> m_list;
};

// Concrete implementation of the PDataList class for QSharedPointer<PModItem>

class PModList : public PDataList<QSharedPointer<PModItem>>
{
    Q_OBJECT
public:
    explicit PModList(QObject *parent = nullptr) : PDataList<QSharedPointer<PModItem>>(parent) {}
};

#endif // PDataList_H
