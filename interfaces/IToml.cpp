#include "IToml.h"

bool IToml::loadConfig(const QString &filePath) {
    // make sure it exists
    if (!QFile::exists(filePath)) {
        return false;
    }

    // read from file
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    // store to byte array
    QByteArray fileData = file.readAll();
    file.close();

    m_toml = toml::parse(fileData.constData());

    return true;
}

bool IToml::saveConfig(const QString &filePath, const toml::table &config) {
    // create dir if it does not exist
    QDir dir(QFileInfo(filePath).absolutePath());
    if (!dir.exists()) {
        dir.mkpath(dir.absolutePath());
    }

    // write to file
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    out << toml::format(config);
    file.close();

    return true;
}

QVariant IToml::getValue(const QString &section, const QString &key) const {
    auto it = m_toml.find(section.toStdString());
    if (it != m_toml.end()) {
        auto keyIt = it->second.find(key.toStdString());
        if (keyIt != it->second.end()) {
            return QString::fromStdString(keyIt->second.as_string().value_or(""));
        }
    }
    return QVariant();
}

void IToml::setValue(const QString &section, const QString &key, const QVariant &value) {
    m_toml[section.toStdString()][key.toStdString()] = value.toString().toStdString();
}

bool IToml::removeValue(const QString &section, const QString &key) {
    auto it = m_toml.find(section.toStdString());
    if (it != m_toml.end()) {
        it->second.erase(key.toStdString());
        return true;
    }
    return false;
}
