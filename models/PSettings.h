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
    Q_PROPERTY(QString pandaHomePath READ pandaHomePath WRITE setPandaHomePath NOTIFY pandaHomePathChanged)

    Q_PROPERTY(int dirty READ dirty WRITE setDirty NOTIFY dirtyChanged)

public:
    explicit PSettings(QObject* parent = nullptr);

    Q_INVOKABLE void updateTable(const QString& key, const QString& value);
    Q_INVOKABLE void updateBoolTable(const QString& key, bool value);
    Q_INVOKABLE void updateStrTable(const QString& key, const QString& value);

    QString getString(const QString& key) const;
    bool getBool(const QString& key) const;

    bool loadFromToml();

    int dirty() const { return m_dirty; }
    void setDirty(int dirty) { m_dirty = dirty; }

signals:
    void zooGamePathChanged();
    void useIsoMountingChanged();
    void isoPathChanged();
    void dirtyChanged(bool dirty);

private:
    QString m_zooGamePath;
    bool m_useIsoMounting = false;
    QString m_isoPath;
    QString m_configPath = QDir::homePath() + "/.panda";
    int m_dirty = 0;
    toml::table m_pandaConfig;
    toml::table m_pandaBackupConfig;
};

#endif // PSETTINGS_H
