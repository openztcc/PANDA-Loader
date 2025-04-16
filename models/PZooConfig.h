#ifndef PZOOCONFIG_H
#define PZOOCONFIG_H

#include <QSettings>
#include <QDir>
#include "../src/PConfigMgr.h"

class PZooConfig : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString zooConfigPath READ getZooConfigPath WRITE setZooConfigPath NOTIFY zooConfigPathChanged)
    Q_PROPERTY(bool dirty READ isDirty WRITE setDirty NOTIFY dirtyChanged)


public:
    explicit PZooConfig(QObject *parent = nullptr, QString zooConfigPath = "");
    PZooConfig(const PZooConfig&) = delete;
    PZooConfig& operator=(const PZooConfig&) = delete;
    PZooConfig(PZooConfig&&) = default;
    ~PZooConfig() = default;

    Q_INVOKABLE QBuffer defaultConfig();
    Q_INVOKABLE void updateTable(const QString &section, const QString &key, const QString &value);
    Q_INVOKABLE void updateTable(const QString &path, const QString &key, bool value);
    Q_INVOKABLE void updateUnlockEntity(const QString &key, const QString &value);
    Q_INVOKABLE void saveConfig();
    Q_INVOKABLE void loadConfig();
    Q_INVOKABLE void revertChanges();
    void removeEmptyKeys(const QString &section, const QString &test);

    // validation
    Q_INVOKABLE bool isInteger(const QString &value, bool test = false) const;
    Q_INVOKABLE bool isFloat(const QString &value, bool test = false, int precision = 0) const;

    bool isDirty() const { return m_dirty; }
    void setDirty(bool dirty) { m_dirty = dirty; }
    

signals:
    void configUpdated(const QString &section, const QString &key, const QString &value);
    void configLoaded(const QString &configPath);
    void configSaved(const QString &configPath);
    void configError(const QString &errorMessage);
    void configReverted();
    void dirtyChanged(bool dirty);
    void saveAttempted

private:
    QString m_zooConfigPath;
    std::unique_ptr<QSettings> m_settings;
    QBuffer m_configBuffer;
    QBuffer m_settingsBackup;
    bool m_dirty;

};

#endif // PZOOCONFIG_H