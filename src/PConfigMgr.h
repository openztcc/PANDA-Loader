#ifndef PCONFIGMGR_H
#define PCONFIGMGR_H

#include "toml.hpp"
#include "PZtdMgr.h"
#include <QSettings>
#include <QDir>
#include <QBuffer>
#include <QIODevice>
#include <QtCore>
#include <SimpleIni.h>
#include "../interfaces/IConfigLoader.h"
#include "PIniConfig.h"
#include "PTomlConfig.h"

class PConfigMgr : QObject {
    Q_OBJECT 
    Q_PROPERTY (int dirty READ isDirty WRITE setDirty NOTIFY dirtyChanged)
public:
    // ----------- Local Models ------------------
    // ini config data
    struct IniData {
        QString filename;
        QString path;
        std::unique_ptr<QSettings> settings;
        QByteArray rawData;

        IniData() = default;

        // operator overloads
        IniData(const IniData&) = delete;
        IniData& operator=(const IniData&) = delete;

        IniData(IniData&&) = default;
        IniData& operator=(IniData&&) = default;
    };

    PConfigMgr(QObject *parent = nullptr, const QString &filepath = "") : QObject(parent) {
        m_configPath = filepath;
        m_config = createParser(m_configPath);
    }

    ~PConfigMgr();

    // meta configuration operations
    bool loadConfig(const QString &filePath);
    bool saveConfig(const QString &filePath);
    QVariant getValue(const QString &section, const QString &key);
    void setValue(const QString &key, const QVariant &value, const QString &section);
    static QVector<QString> getKeyValueAsList(const QString &key, const toml::table &config);
    static bool updateMetaConfig(const QString &ztdFilePath, const toml::table &config);
    static bool removeMetaConfig(const QString &ztdFilePath);

    // asset configuration operations
    static std::vector<std::unique_ptr<PConfigMgr::IniData>> getAllConfigInZtd(const QString &ztdFilePath);
    static std::vector<std::unique_ptr<PConfigMgr::IniData>> getCoreConfigInZtd(const QString &ztdFilePath);
    static std::vector<std::unique_ptr<PConfigMgr::IniData>> getCoreConfigInZtd(std::vector<std::unique_ptr<PConfigMgr::IniData>> &configFiles);
    static std::vector<std::unique_ptr<PConfigMgr::IniData>> getIconAniConfigInZtd(const QString &ztdFilePath);
    static std::vector<std::unique_ptr<PConfigMgr::IniData>> getIconAniConfigInZtd(std::vector<std::unique_ptr<PConfigMgr::IniData>> &configFiles);
    static QStringList getIconAniPaths(const QString &ztdFilePath);
    static QStringList getIconAniPaths(std::vector<std::unique_ptr<PConfigMgr::IniData>> &configFiles);
    static QStringList getIconPaths(std::vector<std::unique_ptr<PConfigMgr::IniData>> &aniFiles);
    static QStringList getIconPaths(const QString &ztdFilePath);

    // setters and getters for QProperties
    int isDirty() const { return m_dirty; }
    void setDirty(bool dirty) { m_dirty = dirty; }

signals:
    void dirtyChanged(int dirty);

private:
    QString m_configPath;
    std::unique_ptr<IConfigLoader> m_config;
    int m_dirty = 0;
    std::unique_ptr<IConfigLoader> createParser(const QString &path) const;
    // helper functions
    static PConfigMgr::IniData byteArrayToIniData(const PZtdMgr::FileData &data);
    static QStringList extractDuplicateKeys(const QByteArray& rawData, const QString& group, const QString& key);
};
#endif // PCONFIGMGR_H
