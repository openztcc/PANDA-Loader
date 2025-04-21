#ifndef IINI_H
#define IINI_H

#include "IConfigLoader.h"
#include "SimpleIni.h"

class IIni : public IConfigLoader {
public:
    IIni() = default;
    bool loadConfig(const QString &filePath) override;
    bool saveConfig(const QString &filePath) override;
    QVariant getValue(const QString &section, const QString &key) const override;
    void setValue(const QString &key, const QVariant &value, const QString &section) override;
    bool removeKey(const QString &section, const QString &key) override;
    bool removeSection(const QString &section) override;
    bool getAllSections() override;
    void removeKeysByValue(const QString &section, const QString &value);

private:
    CSimpleIniA m_ini;
};

#endif // IIni.h
