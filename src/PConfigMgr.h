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
    static bool loadConfig(const QString &filePath);
    static bool saveConfig(const QString &filePath, const toml::table &config);
    static QString getKeyValue(const QString &key, const toml::table &config);
    static bool getBoolValue(const QString &key, const toml::table &config);
    static QVector<QString> getKeyValueAsList(const QString &key, const toml::table &config);
    static bool updateMetaConfig(const QString &ztdFilePath, const toml::table &config);
    static bool removeMetaConfig(const QString &ztdFilePath);
    toml::table getZooIniConfig(const QString &iniPath);

    // zoo.ini configuration operations
    bool updateZooIniConfig(const QString &iniPath, const toml::table &config);
    bool removeZooIniConfig(const QString &iniPath);
    bool readPandaConfig(const QString &filePath, toml::table &config);

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
    QString m_configPath = QDir::homePath() + "/.config/PandaLoader/config.toml";
    std::unique_ptr<IConfigLoader> m_config;
    std::unique_ptr<IConfigLoader> createParser(const QString &ext) const;
    // helper functions
    static PConfigMgr::IniData byteArrayToIniData(const PZtdMgr::FileData &data);
    static QStringList extractDuplicateKeys(const QByteArray& rawData, const QString& group, const QString& key);
};
#endif // PCONFIGMGR_H
