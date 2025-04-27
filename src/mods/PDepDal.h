#ifndef PDEPDAL_H
#define PDEPDAL_H

#include <QCore>

// Project
#include "PDependency.h"
#include "PDatabase.h"

class PDepDal {
    public:
        PDepDal(QObject *parent = nullptr);
        ~PDepDal() = default;
        bool addDependency(const QString &modId, const PDependency &dependency);
        bool removeDependency(const QString &depId);
        bool doesDependencyExist(const QString &dependencyId);
    private: 
        PDatabase m_db;
        const QString m_tableName = "dependencies";
        const QString m_createDependenciesTableQuery =
            "CREATE TABLE IF NOT EXISTS dependencies ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "mod_id TEXT NOT NULL, "
            "dependency_id TEXT NOT NULL, "
            "name TEXT NOT NULL, "
            "min_version TEXT, "
            "optional INTEGER NOT NULL, "
            "ordering TEXT, "
            "link TEXT, "
            "FOREIGN KEY(mod_id) REFERENCES mods(mod_id)"
            ");";

};

#endif // PDEPDAL_H