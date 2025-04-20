#ifndef PCONFIGMGR_H
#define PCONFIGMGR_H

#include "toml.hpp"
#include "PZtdMgr.h"
#include <QSettings>
#include <QDir>
#include <QBuffer>
#include <QIODevice>
#include "../models/PEntityType.h"
#include <QtCore>
#include <SimpleIni.h>
#include "../interfaces/IConfigLoader.h"
#include "../interfaces/IToml.h"
#include "../interfaces/IIni.h"

class PConfigMgr : QObject {
    Q_OBJECT 
    Q_PROPERTY (bool dirty READ isDirty WRITE setDirty NOTIFY dirtyChanged)
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

    PConfigMgr();
    ~PConfigMgr();

    // meta configuration operations
    bool loadConfig(const QString &filePath);
    bool saveConfig(const QString &filePath);
    QVariant getValue(const QString &section, const QString &key);
    void setValue(const QString &section, const QString &key, const QVariant &value);
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
private:
    QString m_configPath;
    std::unique_ptr<IConfigLoader> m_config;
    std::unique_ptr<IConfigLoader> createParser(const QString &ext) const;
    // helper functions
    static PConfigMgr::IniData byteArrayToIniData(const PZtdMgr::FileData &data);
    static QStringList extractDuplicateKeys(const QByteArray& rawData, const QString& group, const QString& key);
};
#endif // PCONFIGMGR_H
