#ifndef PDatabase_H
#define PDatabase_H

// Qt
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVector>
#include <QStringList>
#include <QDir>
#include <QCoreApplication>

enum OrderBy
{
    Ascending,
    Descending
};

enum Operation
{
    Select,
    Insert,
    Update,
    Delete
};

class PDatabase
{
public:
    PDatabase(const QString &dbPath, const QString &connection = "default", const QStringList &tableQueries = {});
    ~PDatabase();

    bool open();
    void close();
    bool createTables(const QStringList &tableQueries);
    bool runQuery(const QString &query);
    QSqlQuery returnQuery(const QString &query);
    bool runQuery(const QString &query, const QVariantMap &params);
    QSqlQuery runOperation(Operation op, const QString &table, const QMap<QString, 
        QVariant> &conditions, const QPair<QString, OrderBy> &orderBy = {}, const QString &groupBy = "",
        const QMap<QString, QVariant> &secondaryConditions = {});

private:
    QSqlDatabase m_db;
    QString m_dbPath;
    QStringList m_tableQueries;

    // helpers
    QString buildSelectQuery(const QString &table, const QMap<QString, QVariant> &conditions, 
        const QPair<QString, OrderBy> &orderBy = {}, const QString &groupBy = "");
    QString buildInsertQuery(const QString &table, const QMap<QString, QVariant> &values);
    QString buildUpdateQuery(const QString &table, const QMap<QString, QVariant> &setFields, 
        const QMap<QString, QVariant> &whereConditions);
    QString buildDeleteQuery(const QString &table, const QMap<QString, QVariant> &conditions);
};

#endif // PDatabase_H
