#ifndef PCONFIGMGR_H
#define PCONFIGMGR_H

#include "toml.hpp"
#include "PZtdMgr.h"
#include <QSettings>
#include <QDir>
#include <QBuffer>
#include <QIODevice>
#include "../models/PEntityType.h"

class PConfigMgr
{
public:
    // ----------- Local Models ------------------
    // ini config data
    struct IniData {
        QString filename;
        QString path;
        QSettings *settings = nullptr;
    };

    PConfigMgr();
    ~PConfigMgr();

    static toml::table getMetaConfig(const QString &ztdFilePath);
    static toml::table getConfig(const QString &filePath);
    static bool saveConfig(const QString &filePath, const toml::table &config);
    static QString getKeyValue(const QString &key, const toml::table &config);
    static bool getBoolValue(const QString &key, const toml::table &config);
    static QVector<QString> getKeyValueAsList(const QString &key, const toml::table &config);
    static bool updateMetaConfig(const QString &ztdFilePath, const toml::table &config);
    static bool removeMetaConfig(const QString &ztdFilePath);
    toml::table getZooIniConfig(const QString &iniPath);
    bool updateZooIniConfig(const QString &iniPath, const toml::table &config);
    bool removeZooIniConfig(const QString &iniPath);
    bool readPandaConfig(const QString &filePath, toml::table &config);

    // misc config
    static QStringList getMenuIconPaths(const QString &ztdFilePath);
    static QStringList getCodenamesInZtd(const QString &ztdFilePath);
private:
    QString m_configPath = QDir::homePath() + "/.config/PandaLoader/config.toml";

    // helper functions
    static QList<std::unique_ptr<PConfigMgr::IniData>> getAllConfigInZtd(const QString &ztdFilePath, const QString &ext = "", const QString &entityType = "");
    static PConfigMgr::IniData byteArrayToIniData(const QByteArray &data);
};
#endif // PCONFIGMGR_H
