#include  "IIni.h"

bool IIni::loadConfig(const QString &filePath) {
    SI_Error rc = m_ini.LoadFile(filePath.toStdString().c_str());
    if (rc < 0) {
        qDebug() << "Failed to load INI file: " << filePath;
        return false;
    }
    return true;
}

bool IIni::saveConfig(const QString &filePath) {
    SI_Error rc = m_ini.SaveFile(filePath.toStdString().c_str());
    if (rc < 0) {
        qDebug() << "Failed to save INI file: " << filePath;
        return false;
    }
    return true;
}

QVariant IIni::getValue(const QString &section, const QString &key) const {
    const char *value = m_ini.GetValue(section.toStdString().c_str(), key.toStdString().c_str(), nullptr);
    if (value) {
        return QString::fromStdString(value);
    }
    return QVariant();
}

void IIni::setValue(const QString &key, const QVariant &value, const QString &section) {
    m_ini.SetValue(section.toStdString().c_str(), key.toStdString().c_str(), value.toString().toStdString().c_str());
}

bool IIni::removeValue(const QString &section, const QString &key) {
    m_ini.DeleteValue(section.toStdString().c_str(), key.toStdString().c_str());
    return true;
}
