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
        bool deleteMod(const QString &table, const QMap<QString, QVariant> &conditions);
        bool updateMod(const QString &table, const QMap<QString, 
            QVariant> &setFields, const QMap<QString, QVariant> &whereConditions);
        bool doesModExist(const QString &modId);
        QSqlQuery getAllMods();
        QVector<QSharedPointer<PModItem>> searchMods(PDatabase::Operation operation, 
            const QString &propertyName, const QString &searchTerm);
        void loadModsFromFile(const QStringList &ztdList);
    private:
        PDatabase m_db;
        const QString m_dbName = "panda.padb";
        PModList
        QSharedPointer<PModItem> m_currentMod;

    };

#endif // PModDal_H