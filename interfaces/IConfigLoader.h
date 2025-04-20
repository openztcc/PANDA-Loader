#include "toml.hpp"
#include "../src/PZtdMgr.h"
#include <QSettings>
#include <QDir>
#include <QBuffer>
#include <QIODevice>
#include "../models/PEntityType.h"
#include <QtCore>
#include <SimpleIni.h>

class IConfigLoader {
    public:
        virtual ~IConfigLoader() = default;
        virtual bool loadConfig(const QString &filePath) = 0;
        virtual bool saveConfig(const QString &filePath, const toml::table &config) = 0;

        virtual QVariant getValue(const QString &section, const QString &key) const = 0;
        virtual void setValue(const QString &section, const QString &key, const QVariant &value) = 0;
        virtual bool removeValue(const QString &section, const QString &key) = 0;
};
