#include "IConfigLoader.h"

class IIni : public IConfigLoader {
    Q_OBJECT
public:
    explicit IIni(QObject *parent = nullptr) : IConfigLoader(parent) {}
    bool loadConfig(const QString &filePath) override;
    bool saveConfig(const QString &filePath) override;
    QVariant getValue(const QString &section, const QString &key) const override;
    void setValue(const QString &section, const QString &key, const QVariant &value) override;
    bool removeValue(const QString &section, const QString &key) override;

private:
    CSimpleIniA m_ini;
};