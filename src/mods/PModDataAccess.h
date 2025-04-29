#ifndef PModDataAccess_H
#define PModDataAccess_H

// This class is the layer between the QML and C++. It is responsible for
// communicating data between the mod loader and the database.
// The mod loader structure is as such:

// - Database, Files, and Configs
// - PModDataAccess, Model
// - PModLoader < ---- facilitates model and db
// - QML

// Qt
#include <QObject>

// Project
#include "PModItem.h"
#include "PDatabase.h"
#include "PModQueries.h"
#include "PConfigMgr.h"

class PModDataAccess : public QObject {

    public:
        // Constructor
        PModDataAccess();
        ~PModDataAccess() = default;

        // Mod operations
        bool insertMod(const PModItem &mod);
        bool deleteMod(const QString &table, const QMap<QString, QVariant> &conditions);
        bool updateMod(const QString &table, const QMap<QString, 
            QVariant> &setFields, const QMap<QString, QVariant> &whereConditions);
        bool doesModExist(const QString &modId);
        QSqlQuery getAllMods();
        QVector<QSharedPointer<PModItem>> searchMods(Operation operation,
            const QString &propertyName, const QString &searchTerm);
        void loadModsFromFile(const QStringList &ztdList);
        QString determineModCategory(const PConfigMgr &config);
        QStringList generateTagsFromConfig(const PConfigMgr &config);
    private:
        PDatabase m_db;
        const QString m_dbName = "panda.padb";
        // PModList
        QSharedPointer<PModItem> m_currentMod;

    };

#endif // PModDataAccess_H                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
