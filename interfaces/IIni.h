#include "IConfigLoader.h"

class IIni : public IConfigLoader {
public:
    IIni() = default;
    bool loadConfig(const QString &filePath) override;
    bool saveConfig(const QString &filePath) override;
    QVariant getValue(const QString &section, const QString &key) const override;
    void setValue(const QString &key, const QVariant &value, const QString &section) override;
    bool removeValue(const QString &section, const QString &key) override;

private:
    CSimpleIniA m_ini;
};
