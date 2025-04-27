#include "PDatabase.h"


// init all db structs
PDatabase::PDatabase() {
    m_dbPath = QCoreApplication::applicationDirPath() + QDir::separator() + m_dbName;
    // remove old connection
    if (QSqlDatabase::contains(m_dbPath)) {
        QSqlDatabase::removeDatabase(m_dbPath);
    }
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(m_dbPath);

    if (!openDatabase()) {
        qDebug() << "Failed to open database";
    }
}

PDatabase::~PDatabase() {}

bool PDatabase::open() {

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
        return createTables();
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

bool PDatabase::doesKeyExist(const QString &modId, const QString &key) {
    QSqlQuery query(m_db);
    
    // Check if mod exists
    if (!doesModExist(modId)) {
        return false;
    }

    // Check if key exists
    query.prepare("SELECT COUNT(*) FROM mods WHERE mod_id = :mod_id AND " + key + " IS NOT NULL");
    query.bindValue(":mod_id", modId);

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
        qDebug() << "Error getting key count: " << query.lastError();
        return false;
    }

    return true;
}

// runs a given query
// usage: runQuery("SELECT * FROM mods WHERE title = 'Mod Title'")
bool PDatabase::runQuery(const QString &query) {
    QSqlQuery q(m_db);
    if (!q.exec(query)) {
        qDebug() << "Error running query: " << q.lastError().text();
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
    QSqlQuery q(m_db);
    q.prepare(query);

    // bind parameters to the query
    for (auto it = params.constBegin(); it != params.constEnd(); ++it) {
        q.bindValue(it.key(), it.value());
    }

    // run prepared query
    if (!q.exec()) {
        qDebug() << "Error running query: " << q.lastError().text();
        return false;
    }
    return true;
}

// Most operations will generally be done in bulk, so this will help when
// user uses things like the filter tags to run one query and get all results
// usage: selectWhere("mods", {{"title", "Mod Title"}, {"enabled", true}}, "title ASC")
QSqlQuery PDatabase::selectWhere(const QString &table, const QMap<QString, QVariant> &conditions, const QPair &orderBy) {
    QSqlQuery query(m_db);

    if (conditions.isEmpty()) {
        qDebug() << "No conditions provided for selectWhere()";
        return query;
    }

    // prepare query str
    QString query = "SELECT * FROM " + table + " WHERE ";
    QStringList whereClauses;

    // build where clause from conditions
    // TODO: add support for other operators (e.g. <, >, !=, etc.)
    // query ex: SELECT * FROM mods WHERE title = :title AND enabled = :enabled
    for (auto it = conditions.constBegin(); it != conditions.constEnd(); ++it) {
        whereClauses.append(it.key() + " = :" + it.key());
    }

    query += whereClauses.join(" AND "); // join with AND

    // if orderBy is not empty, add it to the query
    if (!orderBy.isEmpty()) {
        query += " ORDER BY " + orderBy.first + " " + orderBy.second;
    }

    // prepare the query
    query.prepare(query);

    // bind the parameters to query
    for (auto it = conditions.constBegin(); it != conditions.constEnd(); ++it) {
        query.bindValue(":" + it.key(), it.value());
    }

    // execute query
    if (!query.exec()) {
        qDebug() << "Failed to run selectWhere: " << query.lastError().text();
        return query;
    }

    return query;
}