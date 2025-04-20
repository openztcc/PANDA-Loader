#include "IConfigLoader.h"
#include "toml.hpp"

class IToml : public IConfigLoader {
public:
    IToml() = default;
    bool loadConfig(const QString &filePath) override;
    bool saveConfig(const QString &filePath) override;
    QVariant getValue(const QString &section, const QString &key) const override;
    void setValue(const QString &key, const QVariant &value, const QString &section = "") override;
    bool removeValue(const QString &section, const QString &key) override;

private: 
    toml::table m_toml;
};
