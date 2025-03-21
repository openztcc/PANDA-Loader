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
    static toml::table getMetaConfig(const QString &ztdFilePath);
    static QString getKeyValue(const QString &key, const toml::table &config);
    static QVector<QString> getKeyValueAsList(const QString &key, const toml::table &config);
    static bool updateMetaConfig(const QString &ztdFilePath, const toml::table &config);
    static bool removeMetaConfig(const QString &ztdFilePath);
    toml::table getZooIniConfig(const QString &iniPath);
    bool updateZooIniConfig(const QString &iniPath, const toml::table &config);
    bool removeZooIniConfig(const QString &iniPath);
};
#endif // PCONFIGMGR_H
