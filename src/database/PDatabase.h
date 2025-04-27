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
    QSqlQuery selectWhere(const QString &table, const QMap<QString, QVariant> &conditions, const QPair<QString, OrderBy> &orderBy = {});
    bool doesKeyExist(const QString &modId, const QString &key);

private:
    QSqlDatabase m_db;
    const QString m_dbName = "panda-ldr.padb";
    QString m_dbPath;
};

#endif // PDatabase_H
