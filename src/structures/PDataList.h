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

#include "PAppController.h"

// template class
template <typename T>
class PDataList : public QAbstractListModel
{
    Q_OBJECT
public:
    PDataList(QObject *parent, QStringList ztdList) : QAbstractListModel(parent) {}

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
        if (index.isValid() && index.row() >= 0 && index.row() < m_mods_list.length())
        {
            QSharedPointer<T> item = m_list[index.row()];
            qDebug() << "Fetching data for mod: " << mod->title();
            item->getData(role); // Call the getData method of the item object
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

private:
    QList<QSharedPointer<T>> m_list;
};

#endif // PDataList_H
