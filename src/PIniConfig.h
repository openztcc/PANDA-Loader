#ifndef PINICONFIG_H
#define PINICONFIG_H

#include "../interfaces/IConfigLoader.h"
#include "SimpleIni.h"

class PIniConfig : public IConfigLoader {
public:
    // loading and saving
    PIniConfig() = default;
    bool loadConfig(const QString &filePath) override;
    bool saveConfig(const QString &filePath) override;

    // get and set values
    QVariant getValue(const QString &section, const QString &key) const override;
    void setValue(const QString &key, const QVariant &value, const QString &section) override;
    bool removeKey(const QString &section, const QString &key) override;
    bool removeSection(const QString &section) override;
    bool getAllSections() override;

    // unique to ini config
    void removeKeysByValue(const QString &section, const QString &value);

    // Operator overloads
    PIniConfig& operator=(const PIniConfig& other); // copy assignment operator

    virtual std::unique_ptr<IConfigLoader> clone() const override { // clone
        return std::make_unique<PIniConfig>(*this);
    }

    PIniConfig(const PIniConfig& other) {
        *this = other;
    }

private:
    CSimpleIniA m_ini;

    void PIniConfig::interpretVariant(CSimpleIniA& m_ini, const std::string& key, const QVariant& value);
};

#endif // PIniConfig.h
