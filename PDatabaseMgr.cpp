#include "PDatabaseMgr.h"

PDatabaseMgr::PDatabaseMgr() {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(m_dbName);

    if (!openDatabase()) {
        qDebug() << "Failed to open database";
    }
}

PDatabaseMgr::~PDatabaseMgr() {}

bool PDatabaseMgr::openDatabase() {
    if (!m_db.open()) {
        qDebug() << "Failed to open database: " << m_db.lastError();
        return false;
    }

    return true;
}

void PDatabaseMgr::closeDatabase() {
    m_db.close();
}