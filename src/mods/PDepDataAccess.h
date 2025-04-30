#ifndef PDEPDATAACCESS_H
#define PDEPDATAACCESS_H

#include <QObject>

// Project
#include "PDependency.h"
#include "PDatabase.h"
#include "PModQueries.h"

class PDepDataAccess {
    public:
        PDepDataAccess(const QSharedPointer<PDatabase> &db = nullptr);
        ~PDepDataAccess() = default;
        bool addDependency(const QString &modId, const PDependency &dependency);
        bool removeDependency(const QString &depId);
        bool doesDependencyExist(const QString &dependencyId);
    private: 
        QSharedPointer<PDatabase> m_db;
        const QString m_tableName = "dependencies";
};

#endif // PDEPDATAACCESS_H
