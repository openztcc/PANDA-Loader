#ifndef PZOOCONFIG_H
#define PZOOCONFIG_H

#include <QSettings>
#include <QDir>
#include "../src/PConfigMgr.h"
#include <QBuffer>
#include "SimpleIni.h"

typedef int SI_ERROR;

class PZooConfig : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool dirty READ dirty WRITE setDirty NOTIFY dirtyChanged)


public:
    explicit PZooConfig(QObject *parent = nullptr, QString zooConfigPath = "");

    // Q_INVOKABLE std::unique_ptr<QBuffer> defaultConfig();
    Q_INVOKABLE void updateTable(const QString &section, const QString &key, const QString &value);
    Q_INVOKABLE void updateTable(const QString &path, const QString &key, bool value);
    Q_INVOKABLE void updateUnlockEntity(const QString &key, const QString &value);
    Q_INVOKABLE void saveConfig();
    Q_INVOKABLE void loadConfig();
    Q_INVOKABLE void revertChanges();
    void removeEmptyKeys(const QString &section, const QString &test);
    Q_INVOKABLE bool getBool(const QString &section, const QString &key, const std::unique_ptr<CSimpleIniA> &ini = nullptr) const;
    Q_INVOKABLE QString getString(const QString &section, const QString &key, const std::unique_ptr<CSimpleIniA> &ini = nullptr) const;
    CSimpleIniA getIni() const { return CSimpleIniA(); }
    void copyIni(const std::unique_ptr<CSimpleIniA> &copyFrom, std::unique_ptr<CSimpleIniA> &copyTo) const;

    // validation
    // Q_INVOKABLE bool isInteger(const QString &value, bool test = false) const;
    // Q_INVOKABLE bool isFloat(const QString &value, bool test = false, int precision = 0) const;

    int dirty() const { return m_dirty; }
    void setDirty(int dirty) { m_dirty = dirty; }
    

signals:
    void configUpdated(const QString &section, const QString &key, const QString &value);
    void configLoaded(const QString &configPath);
    void configSaved(const QString &configPath);
    void configError(const QString &errorMessage);
    void configReverted();
    void dirtyChanged(bool dirty);

private:
    QString m_zooConfigPath;
    std::unique_ptr<CSimpleIniA> m_zooBackup;
    std::unique_ptr<CSimpleIniA> m_zooini;
    int m_dirty;

};

#endif // PZOOCONFIG_H
