#ifndef PModDal_H
#define PModDal_H

// This class is the layer between the QML and C++. It is responsible for
// communicating data between the mod loader and the database.
// The mod loader structure is as such:

// - Database, Files, and Configs
// - PModDal, Model
// - PModLoader < ---- facilitates model and db
// - QML

// Qt
#include <QObject>

// Project
#include "PModItem.h"
#include "PDatabase.h"
#include "PModQueries.h"

class PModDal : public QObject {

    public:
        // Constructor
        PModDal();
        ~PModDal() = default;

        // Mod operations
        bool insertMod(const PModItem &mod);
        bool deleteMod(const QString &modId);
        bool updateMod(const QString &modId, const QString &key, const QString &value);
        bool doesModExist(const QString &modId);
        QSqlQuery getAllMods();
        QSqlQuery queryMods(const QString &propertyName, const QString &searchTerm);
        QStringList searchMods(const QString &propertyName, const QString &searchTerm);
        Q_INVOKABLE QSharedPointer<PModItem> getModByPk(const QString &modId);
        QSharedPointer<PModItem> populateModItem(QSqlQuery &query);
        void loadModsFromZTDs(const QStringList &ztdList);
        QSharedPointer<PModItem> queryToModItem(QSqlQuery &query);
        QSharedPointer<PModItem> queryToModItem(QString property, QString value);
        QVector<QSharedPointer<PModItem>> queryToModItems(QString property, QString value);
    private:
        PDatabase m_db;
        const QString m_dbName = "panda.padb";
        PModList
        QSharedPointer<PModItem> m_currentMod;

    };

#endif // PModDal_H