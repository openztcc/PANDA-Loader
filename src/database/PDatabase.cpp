#include "PDatabase.h"


// init all db structs
PDatabase::PDatabase(const QString &dbPath, const QString &connection, const QStringList &tableQueries) 
: m_dbPath(dbPath), m_tableQueries(tableQueries) {
    // remove old connection
    if (QSqlDatabase::contains(m_dbPath)) {
        QSqlDatabase::removeDatabase(m_dbPath);
    }
    m_db = QSqlDatabase::addDatabase("QSQLITE", connection);
    m_db.setDatabaseName(m_dbPath + "/panda.padb");

    // try to open database
    if (!open()) {
        qDebug() << "Failed to open database";
    }
}

PDatabase::~PDatabase() {
    close();
}

bool PDatabase::open() {

    qDebug() << "Trying to open DB at: " << m_dbPath;

    if (!m_db.open()) {
        qDebug() << "Failed to open database: " << m_db.lastError();
        return false;
    }

    // Check if any tables exist
    QSqlQuery query(m_db);
    query.exec("SELECT COUNT(*) FROM sqlite_master WHERE type='table'");

    bool isNewDatabase = true;

    if (query.next()) {
        int tableCount = query.value(0).toInt();
        if (tableCount > 0) {
            isNewDatabase = false;
        }
    } else {
        qDebug() << "Error checking for existing tables:" << query.lastError().text();
        return false;
    }

    if (isNewDatabase) {
        qDebug() << "Database is brand new, initializing tables...";
        return createTables(m_tableQueries);
    }

    return true;
}

void PDatabase::close() {
    m_db.close();
}

// Create tables if they do not exist. Called every time PANDA is started.
// usage: createTables({"CREATE TABLE IF NOT EXISTS mods ......", 
//                     "CREATE TABLE IF NOT EXISTS dependencies ..."})
bool PDatabase::createTables(const QStringList &tableQueries) {
    if (tableQueries.isEmpty()) {
        qDebug() << "No table queries provided to createTables()";
        return false;
    }

    for (const QString &query : tableQueries) {
        QSqlQuery q(m_db);
        if (!q.exec(query)) {
            qDebug() << "Error creating table: " << q.lastError().text();
            return false;
        }
    }

    return true;
}

// runs a given query
// usage: runQuery("SELECT * FROM mods WHERE title = 'Mod Title'")
bool PDatabase::runQuery(const QString &query) {
    QSqlQuery q(m_db);
    if (!q.exec(query)) {
        qDebug() << "Error running runQuery -> query: " << q.lastError().text();
        return false;
    }
    return true;
}

// runs a given query with parameters
// usage: runQuery("INSERT INTO mods (title, authors) VALUES (:title, :authors)", 
//                {{"title", "Mod Title"}, {"authors", "Author1, Author2"}})
// Note: The parameters should be passed as a QVariantMap where the keys are the parameter names
// and the values are the corresponding values to bind to the query.
bool PDatabase::runQuery(const QString &query, const QVariantMap &params) {
    if (!m_db.isOpen()) {
        qDebug() << "ERROR: Database is not open!";
        return false;
    }
    
    QSqlQuery q(m_db);
    q.prepare(query);

    // bind parameters to the query
    for (auto it = params.constBegin(); it != params.constEnd(); ++it) {
        q.bindValue(":" + it.key(), it.value());
    }

    // run prepared query
    if (!q.exec()) {
        qDebug() << "Error running runQuery -> query, params: " << q.lastError().text();
        return false;
    }
    return true;
}

// returnQuery runs a given query and returns the result as a QSqlQuery object
// usage: returnQuery("SELECT * FROM mods WHERE title = 'Mod Title'")
QSqlQuery PDatabase::returnQuery(const QString &query) {
    QSqlQuery q(m_db);
    if (!q.exec(query)) {
        qDebug() << "Error running returnQuery: " << q.lastError().text();
    }
    return q;
}

// Most operations will generally be done in bulk, so this will help when
// user uses things like the filter tags to run one query and get all results
// usage: selectWhere("mods", {{"title", "Mod Title"}, {"enabled", true}}, {"title", OrderBy::Ascending})
// TODO: add limit support later. currently don't see a need.
QSqlQuery PDatabase::runOperation(Operation operation, const QString &table, const QMap<QString, QVariant> &conditions, 
    const QPair<QString, OrderBy> &orderBy, const QString &groupBy, const QMap<QString, QVariant> &secondaryConditions) {
    QSqlQuery query(m_db);

    if (conditions.isEmpty()) {
        qDebug() << "No conditions provided for selectWhere()";
        return query;
    }

    // prepare query str
    QString queryStr;
    switch (operation) {
        case Operation::Select:
            queryStr = buildSelectQuery(table, conditions, orderBy, groupBy);
            break;
        case Operation::Insert:
            queryStr = buildInsertQuery(table, conditions);
            break;
        case Operation::Update:
            queryStr = buildUpdateQuery(table, conditions, secondaryConditions);
            break;
        case Operation::Delete:
            queryStr = buildDeleteQuery(table, conditions);
            break;
    }

    // prepare query
    query.prepare(queryStr);

    // bind parameters to the query
    for (auto it = conditions.constBegin(); it != conditions.constEnd(); ++it) {
        query.bindValue(":" + it.key(), it.value());
    }

    // execute query
    if (!query.exec()) {
        qDebug() << "Failed to run query: " << query.lastQuery();
        qDebug() << "Bound values: " << conditions;
        qDebug() << "Failed to run selectWhere: " << query.lastError().text();
        return query;
    }

    return query;
}

// -------------------- HELPERS

// build select query string
// usage: buildSelectQuery("mods", {{"title", "Mod Title"}, {"enabled", true}}, {"title", OrderBy::Ascending})
// returns: SELECT * FROM mods WHERE title = :title AND enabled = :enabled ORDER BY title ASC
QString PDatabase::buildSelectQuery(const QString &table, const QMap<QString, QVariant> &conditions, 
    const QPair<QString, OrderBy> &orderBy, const QString &groupBy) {
    QString queryStr = "SELECT * FROM " + table + " WHERE ";
    QStringList whereClauses;

    // remove any empty keys in conditions
    QMap<QString, QVariant> filteredConditions = conditions;
    for (auto it = filteredConditions.constBegin(); it != filteredConditions.constEnd();) {
        if (it.key().isEmpty() || it.key() == "") {
            it = filteredConditions.erase(it); // remove empty keys
        } else {
            ++it;
        }
    }

    // build where clause from conditions
    for (auto it = conditions.constBegin(); it != conditions.constEnd(); ++it) {
        whereClauses.append(it.key() + " = :" + it.key());
    }

    queryStr += whereClauses.join(" AND "); // join with AND

    // if orderBy is not empty, add it to the query
    if (!orderBy.first.isEmpty()) {
        switch (orderBy.second) {
            case OrderBy::Ascending:
                queryStr += " ORDER BY " + orderBy.first + " ASC";
                break;
            case OrderBy::Descending:
                queryStr += " ORDER BY " + orderBy.first + " DESC";
                break;
        }
    }

    // if groupBy is not empty, add it to the query
    if (!groupBy.isEmpty()) {
        queryStr += " GROUP BY " + groupBy;
    }

    return queryStr;
}

// build insert query string
// usage: buildInsertQuery("mods", {{"title", "Mod Title"}, {"enabled", true}})
// returns: INSERT INTO mods (title, enabled) VALUES (:title, :enabled)
QString PDatabase::buildInsertQuery(const QString &table, const QMap<QString, QVariant> &values) {
    QString queryStr = "INSERT INTO " + table + " (";
    QStringList valueClauses;

    // build value clause from values
    for (auto it = values.constBegin(); it != values.constEnd(); ++it) {
        queryStr += it.key() + ", ";
        valueClauses.append(":" + it.key());
    }

    // remove last comma and space
    queryStr.chop(2);
    queryStr += ") VALUES (" + valueClauses.join(", ") + ")";

    return queryStr;
}

// build update query string
// usage: buildUpdateQuery("mods", {{"title", "Mod Title"}, {"enabled", true}}, {{"mod_id", "123"}})
// returns: UPDATE mods SET title = :title, enabled = :enabled WHERE mod_id = :mod_id
// note: conditions is useful as a QMap because it can, for example, edit author names
// for multiple mods at once. Will be lots of use cases where author is unknown by default.
QString PDatabase::buildUpdateQuery(const QString &table, const QMap<QString, QVariant> &setFields, 
    const QMap<QString, QVariant> &whereConditions) {
    QString queryStr = "UPDATE " + table + " SET ";
    QStringList setClauses;
    QStringList whereClauses;

    // build set clause from values
    for (auto it = whereConditions.constBegin(); it != whereConditions.constEnd(); ++it) {
        setClauses.append(it.key() + " = :" + it.key());
    }

    queryStr += setClauses.join(", "); // join with comma

    // build where clause from conditions
    for (auto it = whereConditions.constBegin(); it != whereConditions.constEnd(); ++it) {
        whereClauses.append(it.key() + " = :" + it.key());
    }

    queryStr += " WHERE " + whereClauses.join(" AND "); // join with AND

    return queryStr;
}

// build delete query string
// usage: buildDeleteQuery("mods", {{"mod_id", "123"}})
// returns: DELETE FROM mods WHERE mod_id = :mod_id
QString PDatabase::buildDeleteQuery(const QString &table, const QMap<QString, QVariant> &conditions) {
    QString queryStr = "DELETE FROM " + table + " WHERE ";
    QStringList whereClauses;

    // build where clause from conditions
    for (auto it = conditions.constBegin(); it != conditions.constEnd(); ++it) {
        whereClauses.append(it.key() + " = :" + it.key());
    }

    queryStr += whereClauses.join(" AND "); // join with AND

    return queryStr;
}
