#ifndef ITOML_H
#define ITOML_H

#include "IConfigLoader.h"
#include "toml.hpp"

class IToml : public IConfigLoader {
public:
    IToml() = default;
    bool loadConfig(const QString &filePath) override;
    bool saveConfig(const QString &filePath) override;
    QVariant getValue(const QString &section, const QString &key) const override;
    void setValue(const QString &key, const QVariant &value, const QString &section = "") override;
    bool removeKey(const QString &key, const QString &section = "") override;
    bool removeSection(const QString &section) override;
    bool getAllSections() override;

private: 
    toml::table m_toml;

protected:
    void interpretVariant(toml::table& config, const std::string& key, const QVariant& value);
    void appendVariantToArray(toml::array& arr, const QVariant& value);
};

#endif // IToml.h
