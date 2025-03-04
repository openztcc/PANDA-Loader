#ifndef PCONFIGMGR_H
#define PCONFIGMGR_H

#include "toml.hpp"
#include "PZtdMgr.h"
#include <QSettings>
#include <QDir>

class PConfigMgr
{
public:
    PConfigMgr();
    ~PConfigMgr();
    toml::table getMetaConfig(const QString &ztdFilePath);
    static QString getKeyValue(const QString &key, const toml::table &config);
    bool updateMetaConfig(const QString &ztdFilePath, const toml::table &config);
    bool removeMetaConfig(const QString &ztdFilePath);
    bool addMetaConfig(const QString &ztdFilePath, const toml::table &config);
    toml::table getZooIniConfig(const QString &iniPath);
    bool updateZooIniConfig(const QString &iniPath, const toml::table &config);
    bool removeZooIniConfig(const QString &iniPath);

private:
    PZtdMgr m_ztdMgr;
    QSettings m_settings;
    const QString m_zooIniName = "zoo.ini";
    const QString m_metaConfigName = "meta.toml";
    const QString m_configDirPath = QDir::homePath() + "/.config/PandaLdr/"; // temporary
    const QString m_metaConfigDirPath = m_configDirPath + "meta_configs/"; // temporary
};
    #endif // PCONFIGMGR_H
