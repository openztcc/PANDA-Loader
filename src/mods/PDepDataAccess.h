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
        bool addDependency(const QMap<QString, QString> &dependency);
        bool removeDependency(const QString &depId);
        bool doesDependencyExist(const QString &dependencyId);


    private: 
        QSharedPointer<PDatabase> m_db;

        QString m_dbName = "panda.padb";

};

#endif // PDEPDATAACCESS_H
