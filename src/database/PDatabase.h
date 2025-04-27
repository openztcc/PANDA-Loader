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

class PDatabase : public QObject
{
    Q_OBJECT
public:
    PDatabase();
    ~PDatabase();

    bool open();
    void close();
    bool createTables(const QStringList &tableQueries);
    bool runQuery(const QString &query);
    bool runQuery(const QString &query, const QVariantMap &params);
    QSqlQuery runOperation(Operation op, const QString &table, const QMap<QString, 
        QVariant> &conditions, const QPair<QString, OrderBy> &orderBy = {}, const QString &groupBy = "",
        const QMap<QString, QVariant> &secondaryConditions = {});
    bool delete
    bool doesKeyExist(const QString &modId, const QString &key);

private:
    QSqlDatabase m_db;
    const QString m_dbName = "panda-ldr.padb";
    QString m_dbPath;

    // helpers
    QString buildSelectQuery(const QString &table, const QMap<QString, QVariant> &conditions, 
        const QPair<QString, OrderBy> &orderBy = {}, const QString &groupBy = "");
    QString buildInsertQuery(const QString &table, const QMap<QString, QVariant> &values);
    QString buildUpdateQuery(const QString &table, const QMap<QString, QVariant> &setFields, 
        const QMap<QString, QVariant> &whereConditions);
    QString buildDeleteQuery(const QString &table, const QMap<QString, QVariant> &conditions);
};

#endif // PDatabase_H
