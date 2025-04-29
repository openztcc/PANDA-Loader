#ifndef PDataList_H
#define PDataList_H

// This is a generic implementation of a QAbstractListModel class that is meant to be 
// reused for any data type. Ideally any type that uses it must define both the getData() and
// roleNames() methods. Additionally, a QAbstractListModel-compatible rolenames enum struct
// must be defined in the class that uses this template.

// Qt
#include <QObject>
#include <QAbstractListModel>
#include <QSharedPointer>

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
            QSharedPointer<T> item = m_list[index.row()];
            item->getData(role);
            return item->getData(role);
        }

        // Return empty if the index is invalid
        qDebug() << "Invalid index: " << index.row() << ", returning empty QVariant";
        return QVariant();
    }

    // return the role names for the model
    QHash<int, QByteArray> roleNames() const
    {
        return T::roleNames();
    }

    // add item to the list
    void addItem(QSharedPointer<T> item)
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
    void removeItems(QList<QSharedPointer<T>> items)
    {
        beginResetModel();
        for (const auto& item : items) {
            m_list.removeOne(item);
        }
        endResetModel();
    }

    // replace item in the list
    void replaceItem(int row)
    {
        if (row >= 0 && row < m_list.size()) {
            QModelIndex index = this->index(row);
            emit dataChanged(index, index);
        }
    }

    void replaceList(QList<QSharedPointer<T>> list)
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

private:
    QList<QSharedPointer<T>> m_list;
};

#endif // PDataList_H
