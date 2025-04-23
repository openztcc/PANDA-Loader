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

    // Exist tests
    bool sectionExists(const QString &section) const override;
    bool keyExists(const QString &key, const QString &section) const override;
    bool valueExists(const QString &value, const QString &key, const QString &section) const override;

    // Operator overloads
    PTomlConfig& operator=(const PTomlConfig& other) { // Copy assignment operator
        if (this != &other) {
            m_toml.clear();
            std::stringstream ss;
            ss << other.m_toml;
            m_toml = toml::parse(ss.str());
        }
        return *this;
    }

    virtual std::unique_ptr<IConfigLoader> clone() const override {
        return std::make_unique<PTomlConfig>(*this);
    }

    PTomlConfig(const PTomlConfig& other) {
        m_toml.clear();
        std::stringstream ss;
        ss << other.m_toml;
        m_toml = toml::parse(ss.str());
    }
    QVariant extractVariant(const QString& node) const override;
    QVariant extractVariant(const toml::node& node) const;

private:
    toml::table m_toml;

protected:
    void interpretVariant(toml::table& config, const std::string& key, const QVariant& value);
    void appendVariantToArray(toml::array& arr, const QVariant& value);
};

#endif // PTomlConfig.h
