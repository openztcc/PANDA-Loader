#include "IConfigLoader.h"

class IToml : public IConfigLoader {
    Q_OBJECT
public:
    explicit IToml(QObject *parent = nullptr) : IConfigLoader(parent) {}
    bool loadConfig(const QString &filePath) override;
    bool saveConfig(const QString &filePath, const toml::table &config) override;
    QVariant getValue(const QString &section, const QString &key) const override;
    void setValue(const QString &section, const QString &key, const QVariant &value) override;
    bool removeValue(const QString &section, const QString &key) override;

private: 
    toml::table m_toml;
};