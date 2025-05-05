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
#include "PFile.h"

class PModDataAccess : public QObject {

    public:
        // Constructor
        PModDataAccess(QObject *parent = nullptr, QSharedPointer<PDatabase> db = nullptr);
        ~PModDataAccess() = default;

        // Mod load operations
        bool insertMod(QSharedPointer<PModItem> mod);
        bool deleteMod(const QString &table, const QMap<QString, QVariant> &conditions);
        bool updateMod(const QString &table, const QMap<QString, 
            QVariant> &setFields, const QMap<QString, QVariant> &whereConditions);
        bool doesModExist(const QString &modId);
        QVector<QSharedPointer<PModItem>> getAllMods(const QString &orderBy,  OrderBy direction = OrderBy::Ascending, const QPair<QString, QVariant> &exception = {});
        QVector<QSharedPointer<PModItem>> searchMods(Operation operation,
            const QString &propertyName, const QString &searchTerm);
        QVector<QSharedPointer<PModItem>> searchMods(Operation operation,
            const QMap<QString, QVariant> &conditions);
        void loadModsFromFile(const QStringList &ztdList);
        bool getFlag(const QString &modId, const QString &flagName);

        // dependencies
        bool insertDependency(const QMap<QString, QVariant> &dependency);
        bool removeDependency(const QString &modId, const QString &depId);

        QSharedPointer<PDatabase> getDatabase() const {
            return m_db;
        }

    private:
        QSharedPointer<PDatabase> m_db;

        const QString m_dbName = "panda.padb";
        // PModList
        QSharedPointer<PModItem> m_currentMod;

    };

#endif // PModDataAccess_H                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
