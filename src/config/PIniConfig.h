#ifndef PINICONFIG_H
#define PINICONFIG_H

// Project
#include "IConfigLoader.h"

// External
#include "SimpleIni.h"

class PIniConfig : public IConfigLoader {
public:
    // loading and saving
    PIniConfig() = default;
    bool loadConfig(const QString &filePath) override;
    bool saveConfig(const QString &filePath) override;
    bool clear() override;

    // get and set values
    QVariant getValue(const QString &section, const QString &key) const override;
    void setValue(const QString &key, const QVariant &value, const QString &section) override;
    bool removeKey(const QString &key, const QString &section = "") override;
    bool removeSection(const QString &section) override;
    bool getAllSections() override;

    // exist tests
    bool sectionExists(const QString &section) const override;
    bool keyExists(const QString &key, const QString &section) const override;
    bool valueExists(const QString &value, const QString &key, const QString &section) const override;

    // unique to ini config
    void removeKeysByValue(const QString &section, const QString &value);

    // Operator overloads
    PIniConfig& operator=(const PIniConfig& other); // copy assignment operator

    virtual std::unique_ptr<IConfigLoader> clone() const override { // clone
        return std::make_unique<PIniConfig>(*this);
    }

    PIniConfig(const PIniConfig& other) {
        m_ini.Reset();
        m_ini.SetUnicode(other.m_ini.IsUnicode());
    
        CSimpleIniA::TNamesDepend sections;
        other.m_ini.GetAllSections(sections);
        for (const auto& section : sections) {
            CSimpleIniA::TNamesDepend keys;
            other.m_ini.GetAllKeys(section.pItem, keys);
            for (const auto& key : keys) {
                const char* value = other.m_ini.GetValue(section.pItem, key.pItem, nullptr);
                if (value) {
                    m_ini.SetValue(section.pItem, key.pItem, value);
                }
            }
        }
    }
    
    QVariant extractVariant(const QString& variant) const override;
private:
    CSimpleIniA m_ini;

    void interpretVariant(CSimpleIniA& m_ini, const std::string& section, const std::string& key, const QVariant& value);
};

#endif // PIniConfig.h
