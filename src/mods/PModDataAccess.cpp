#include "PModDataAccess.h"

PModDataAccess::PModDataAccess(QObject *parent, QSharedPointer<PDatabase> db) {
    m_db = db;
}

bool PModDataAccess::insertMod(QSharedPointer<PModItem> mod) 
{
    QVariantMap params;

    // mod properties
    params.insert("title", mod->title());
    params.insert("authors", mod->authors().join(", "));
    params.insert("description", mod->description());
    params.insert("version", mod->version());
    params.insert("mod_id", mod->id());
    params.insert("link", mod->link());

    // mod flags
    params.insert("enabled", mod->enabled() ? 1 : 0);
    params.insert("is_selected", mod->selected() ? 1 : 0);
    params.insert("listed", mod->listed() ? 1 : 0);
    params.insert("is_collection", mod->isCollection() ? 1 : 0);

    // categorization properties
    params.insert("category", mod->category());
    params.insert("tags", mod->tags().join(", "));

    // external properties
    params.insert("dep_id", mod->dependencyId());
    params.insert("collection_id", mod->collectionId());

    // file data properties
    params.insert("filename", mod->filename());
    params.insert("current_location", mod->currentLocation());
    params.insert("original_location", mod->originalLocation());
    params.insert("disabled_location", mod->disabledLocation());
    params.insert("file_size", mod->fileSize());
    params.insert("file_date", mod->fileDate());

    // graphics properties
    params.insert("icon_paths", mod->iconpaths().join(", "));
    
    return m_db->runQuery(PQueries::ModsInsertQuery, params);
}

bool PModDataAccess::deleteMod(const QString &table, const QMap<QString, QVariant> &conditions) {
    QSqlQuery deletedMods = m_db->runOperation(Operation::Delete, table, conditions);
    if (deletedMods.lastError().isValid()) {
        qDebug() << "Error deleting mod: " << deletedMods.lastError().text();
        return false;
    }
    return true;
}

bool PModDataAccess::updateMod(const QString &table, const QMap<QString, QVariant> &setFields, const QMap<QString, QVariant> &whereConditions) {
    QSqlQuery updatedMods = m_db->runOperation(Operation::Update, table, setFields, {}, "", whereConditions);
    if (updatedMods.lastError().isValid()) {
        qDebug() << "Error updating mod: " << updatedMods.lastError().text();
        return false;
    }
    return true;
}

bool PModDataAccess::doesModExist(const QString &modId) {
    QSqlQuery doesExist = m_db->runOperation(Operation::Select, "mods", {{"mod_id", modId}});
    if (doesExist.lastError().isValid()) {
        qDebug() << "Error checking if mod exists: " << doesExist.lastError().text();
        return false;
    }
    if (doesExist.size() > 0) {
        return true; // Mod exists
    }

    return false;
}

// TODO: update runOperation to handle select all queries so that user can potentially
// use orderBy and groupBy to get all mods in a certain order or group
QVector<QSharedPointer<PModItem>> PModDataAccess::getAllMods(const QString &orderBy,  OrderBy direction, const QPair<QString, QVariant> &exception) {
    QString dir;
    if (direction == OrderBy::Ascending) {
        dir = "ASC";
    } else if (direction == OrderBy::Descending) {
        dir = "DESC";
    } else {
        dir = "ASC"; // def ascending
    }

    QSqlQuery query;
    if (exception.first.isEmpty()) {
        query = m_db->returnQuery("SELECT * FROM mods ORDER BY " + orderBy + " " + dir);
    } else { // return mods without the exception key/value
        query = m_db->returnQuery("SELECT * FROM mods WHERE " + exception.first + " != " + exception.second.toString() + " ORDER BY " + orderBy + " " + dir);
    }

    // check for errors
    if (query.lastError().isValid()) {
        qDebug() << "Error getting all mods: " << query.lastError().text();
        return QVector<QSharedPointer<PModItem>>(); // return empty vector on error
    }

    QVector<QSharedPointer<PModItem>> modItems = QVector<QSharedPointer<PModItem>>();
    while (query.next()) {
        QSharedPointer<PModItem> modItem = QSharedPointer<PModItem>::create(this, query);
        modItems.append(modItem);
    }
    qDebug() << "getAllMods: Found " << modItems.size() << " mods in database.";
    return modItems;
}

// Get search results as a list of strings
QVector<QSharedPointer<PModItem>> PModDataAccess::searchMods(Operation operation, const QString &propertyName, const QString &searchTerm) {
    QSqlQuery query = m_db->runOperation(operation, "mods", {{propertyName, searchTerm}});
    QVector<QSharedPointer<PModItem>> modItems;

    while (query.next()) {
        QSharedPointer<PModItem> modItem = QSharedPointer<PModItem>::create(this, query);
        modItems.append(modItem);
    }
    return modItems;
}

// Get search results as a list of strings
QVector<QSharedPointer<PModItem>> PModDataAccess::searchMods(Operation operation, const QMap<QString, QVariant> &conditions) {
    QSqlQuery query = m_db->runOperation(operation, "mods", conditions);
    QVector<QSharedPointer<PModItem>> modItems = QVector<QSharedPointer<PModItem>>();

    while (query.next()) {
        QSharedPointer<PModItem> modItem = QSharedPointer<PModItem>::create(this, query);
        modItems.append(modItem);
    }

    return modItems;
}

// Insert mod dependencies into the database
bool PModDataAccess::insertDependency(const QMap<QString, QVariant> &dependency) {
    if (!m_db->runQuery(PQueries::DependencyInsertQuery, dependency)) {
        qDebug() << "Failed to insert dependency: " << dependency["name"];
        return false;
    }
    return true;
}

// Delete mod dependencies from the database
bool PModDataAccess::removeDependency(const QString &modId, const QString &depId) {
    m_db->runOperation(Operation::Delete, PQueries::DependenciesTable, {{"mod_id", modId}, {"dependency_id", depId}});
    return true;
}

// Get the flag value for a mod
bool PModDataAccess::getFlag(const QString &modId, const QString &flagName) {
    QSqlQuery query = m_db->runOperation(Operation::Select, "mods", {{"mod_id", modId}});
    if (query.lastError().isValid()) {
        qDebug() << "Error getting flag: " << query.lastError().text();
        return false;
    }
    if (query.next()) {
        return query.value(flagName).toBool();
    }
    return false;
}
