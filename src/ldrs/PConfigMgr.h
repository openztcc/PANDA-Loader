#ifndef PCONFIGMGR_H
#define PCONFIGMGR_H

// Qt
#include <QSettings>
#include <QDir>
#include <QBuffer>
#include <QIODevice>
#include <QtCore>

// External
#include <SimpleIni.h>
#include "toml.hpp"

// Project
#include "PZtdMgr.h"
#include "../interfaces/IConfigLoader.h"
#include "../models/PFileData.h"
#include "PIniConfig.h"
#include "PTomlConfig.h"

class PConfigMgr : public QObject {
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

    PConfigMgr(QObject *parent = nullptr, const QString &filepath = "");
    PConfigMgr(QObject *parent, const PFileData &fileData = PFileData());

    ~PConfigMgr();

    // meta configuration operations
    bool loadConfig(const QString &filePath);
    bool loadConfig(const PFileData &fileData);
    bool saveConfig(const QString &filePath);
    Q_INVOKABLE bool saveConfig();
    Q_INVOKABLE bool revertChanges();
    bool clear();
    Q_INVOKABLE QVariant getValue(const QString &section, const QString &key);
    Q_INVOKABLE void setValue(const QString &key, const QVariant &value, const QString &section);
    static QVector<QString> getKeyValueAsList(const QString &key, const toml::table &config);
    static bool updateMetaConfig(const QString &ztdFilePath, const toml::table &config);
    static bool removeMetaConfig(const QString &ztdFilePath);

    // setters and getters for QProperties
    int isDirty() const { return m_dirty; }
    void setDirty(bool dirty) { m_dirty = dirty; }

    // operator overloads
    PConfigMgr& operator=(const PConfigMgr& other) {
        if (this != &other) {
            m_configPath = other.m_configPath;
            m_config = other.m_config ? other.m_config->clone() : nullptr;
            m_dirty = other.m_dirty;
            m_dirty_laundry = other.m_dirty_laundry ? other.m_dirty_laundry->clone() : nullptr;
        }
        return *this;
    }

signals:
    void dirtyChanged(int dirty);

private:
    QString m_configPath;
    std::unique_ptr<IConfigLoader> m_config;
    std::unique_ptr<IConfigLoader> m_configBackup;
    std::unique_ptr<IConfigLoader> m_dirty_laundry;
    int m_dirty;
    std::unique_ptr<IConfigLoader> createParser(const QString &path) const;
    // helper functions
    static PConfigMgr::IniData byteArrayToIniData(const PZtdMgr::FileData &data);
    static QStringList extractDuplicateKeys(const QByteArray& rawData, const QString& group, const QString& key);
};
#endif // PCONFIGMGR_H
