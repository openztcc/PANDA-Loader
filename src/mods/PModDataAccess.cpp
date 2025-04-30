#include "PModDataAccess.h"

PModDataAccess::PModDataAccess(QObject *parent, QSharedPointer<PDatabase> db) {
    if (!m_db) {
        m_db = QSharedPointer<PDatabase>::create(this, m_dbName, "ModDal");
    }
}

bool PModDataAccess::insertMod(QSharedPointer<PModItem> mod) 
{
    QVariantMap params;
    params.insert(":title", mod->title());
    params.insert(":authors", mod->authors().join(", "));
    params.insert(":description", mod->description());
    params.insert(":enabled", mod->enabled() ? 1 : 0);
    params.insert(":category", mod->category());
    params.insert(":tags", mod->tags().join(", "));
    params.insert(":version", mod->version());
    params.insert(":mod_id", mod->id());
    params.insert(":dep_id", mod->dependencyId());
    params.insert(":filename", mod->filename());
    params.insert(":current_location", mod->currentLocation());
    params.insert(":iconpaths", mod->iconpaths().join(", "));
    params.insert(":original_location", mod->originalLocation());
    params.insert(":is_selected", mod->selected() ? 1 : 0);
    
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
QVector<QSharedPointer<PModItem>> PModDataAccess::getAllMods(const OrderBy &orderBy, const QPair<QString, QVariant> &exception) {
    QString order;
    if (orderBy == OrderBy::Ascending) {
        order = "ASC";
    } else if (orderBy == OrderBy::Descending) {
        order = "DESC";
    } else {
        order = "ASC"; // def ascending
    }

    QSqlQuery query;
    if (exception.first.isEmpty()) {
        query = m_db->returnQuery("SELECT * FROM mods ORDER BY " + order);
    } else { // return mods without the exception key/value
        query = m_db->returnQuery("SELECT * FROM mods WHERE " + exception.first + " != " + exception.second.toString() + " ORDER BY " + order);
    }

    QVector<QSharedPointer<PModItem>> modItems = QVector<QSharedPointer<PModItem>>();
    while (query.next()) {
        QSharedPointer<PModItem> modItem = QSharedPointer<PModItem>::create(this, query);
        modItems.append(modItem);
    }
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
