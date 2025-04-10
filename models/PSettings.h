#ifndef PSETTINGS_H
#define PSETTINGS_H

#include <QObject>
#include <QString>
#include "toml.hpp"
#include <QDir>
#include <QDebug>
#include "../src/PConfigMgr.h"

class PSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString zooGamePath READ zooGamePath WRITE setZooGamePath NOTIFY zooGamePathChanged)
    Q_PROPERTY(bool useIsoMounting READ useIsoMounting WRITE setUseIsoMounting NOTIFY useIsoMountingChanged)
    Q_PROPERTY(QString isoPath READ isoPath WRITE setIsoPath NOTIFY isoPathChanged)

public:
    explicit PSettings(QObject* parent = nullptr);

    QString zooGamePath() const { return m_zooGamePath; }
    void setZooGamePath(const QString& path);

    QString pandaHomePath() const { return m_configPath; }
    void setPandaHomePath(const QString& path) { m_configPath = path; }

    bool useIsoMounting() const { return m_useIsoMounting; }
    void setUseIsoMounting(bool use);

    QString isoPath() const { return m_isoPath; }
    void setIsoPath(const QString& path);

    bool loadFromToml();

signals:
    void zooGamePathChanged();
    void useIsoMountingChanged();
    void isoPathChanged();

private:
    QString m_zooGamePath;
    bool m_useIsoMounting = false;
    QString m_isoPath;
    QString m_configPath = QDir::homePath() + "/.panda";
};

#endif // PSETTINGS_H
