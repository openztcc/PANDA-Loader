#include "PDepDataAccess.h"

PDepDataAccess::PDepDataAccess(const QSharedPointer<PDatabase> &db) {
    m_db = db;
    if (!m_db) {
        qDebug() << "Database is not initialized.";
        return;
    }
}

bool PDepDataAccess::addDependency(const QString &modId, const PDependency &dependency) {
    QSqlQuery query(m_db);

    // Check if mod exists
    if (!doesModExist(modId)) {
        return false;
    }
    
    // Insert dependency data into dependencies table
    query.prepare("INSERT INTO dependencies (mod_id, dependency_id, name, min_version, optional, ordering, link) "
                  "VALUES (:mod_id, :dependency_id, :name, :min_version, :optional, :ordering, :link)");
    
    if (dependency.modId.isEmpty() || modId.isEmpty() || dependency.dependencyId.isEmpty()) {
        qDebug() << "Dependency modId is empty";
        return false;
    }
    else {
        query.bindValue(":mod_id", modId);
        query.bindValue(":dependency_id", dependency.modId);
    }

    if (dependency.name.isEmpty()) {
        query.bindValue(":name", "");
    } 
    else {
        query.bindValue(":name", dependency.name);
    }

    if (dependency.min_version.isEmpty()) {
        query.bindValue(":min_version", "");
    } 
    else {
        query.bindValue(":min_version", dependency.min_version);
    }

    if (dependency.optional) {
        query.bindValue(":optional", 1);
    } 
    else {
        query.bindValue(":optional", 0);
    }

    if (dependency.ordering.isEmpty()) {
        query.bindValue(":ordering", "");
    } 
    else {
        query.bindValue(":ordering", dependency.ordering);
    }

    if (dependency.link.isEmpty()) {
        query.bindValue(":link", "");
    } 
    else {
        query.bindValue(":link", dependency.link);
    }

    if (!query.exec()) {
        qDebug() << "Failed to add dependency: " << query.lastError();
        return false;
    }

    return true;
}

//TODO: Fix so it removes dependency from mod, not just dependencies table
bool PDepDataAccess::removeDependency(const QString &dependencyId) {
    QSqlQuery query(m_db);

    // Remove the dependency from dependencies table
    query.prepare("DELETE FROM dependencies WHERE dependency_id = :dependency_id");
    query.bindValue(":dependency_id", dependencyId);

    if (!query.exec()) {
        qDebug() << "Failed to remove dependency: " << query.lastError();
        return false;
    }

    return true;
}

bool PDepDataAccess::doesDependencyExist(const QString &dependencyId) {
    QSqlQuery query(m_db);

    // Check if dependency exists
    query.prepare("SELECT COUNT(*) FROM dependencies WHERE dependency_id = :dependency_id");
    query.bindValue(":dependency_id", dependencyId);

    if (!query.exec()) {
        qDebug() << "Error running query: " << query.lastError();
        return false;
    }

    if (query.next()) {
        int count = query.value(0).toInt();
        if (count == 0) {
            return false;
        }
    }
    else {
        qDebug() << "Error getting dependency count: " << query.lastError();
        return false;
    }

    return true;
}
