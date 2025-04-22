#ifndef PTOMLCONFIG_H
#define PTOMLCONFIG_H

#include "../interfaces/IConfigLoader.h"
#include "toml.hpp"

class PTomlConfig : public IConfigLoader {
public:
    // Loading and saving
    PTomlConfig() = default;
    bool loadConfig(const QString &filePath) override;
    bool saveConfig(const QString &filePath) override;
    bool clear() override;

    // Get and set values
    QVariant getValue(const QString &section, const QString &key) const override;
    void setValue(const QString &key, const QVariant &value, const QString &section = "") override;
    bool removeKey(const QString &key, const QString &section = "") override;
    bool removeSection(const QString &section) override;
    bool getAllSections() override;

    // Operator overloads
    PTomlConfig& operator=(const PTomlConfig& other) { // Copy assignment operator
        if (this != &other) {
            m_toml = other.m_toml;
        }
        return *this;
    }

    virtual std::unique_ptr<IConfigLoader> clone() const override {
        return std::make_unique<PTomlConfig>(*this);
    }

private: 
    toml::table m_toml;

protected:
    void interpretVariant(toml::table& config, const std::string& key, const QVariant& value);
    void appendVariantToArray(toml::array& arr, const QVariant& value);
    QVariant extractVariant(const toml::node& node) const;
};

#endif // PTomlConfig.h
