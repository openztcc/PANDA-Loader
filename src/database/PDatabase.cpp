#include "PDatabase.h"



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

bool PDatabase::openDatabase() {

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

void PDatabase::closeDatabase() {
    m_db.close();
}

bool PDatabase::createTables() {
    QSqlQuery query(m_db);
    if (!query.exec(m_createTableQuery)) {
        qDebug() << "Failed to create table: " << query.lastError();
        return false;
    }

    if (!query.exec(m_createDependenciesTableQuery)) {
        qDebug() << "Failed to create dependencies table: " << query.lastError();
        return false;
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
bool PDatabase::runQuery(const QString &query) {
    QSqlQuery q(m_db);
    if (!q.exec(query)) {
        qDebug() << "Error running query: " << q.lastError().text();
        return false;
    }
    return true;
}

// runs a given query with parameters
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