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

bool IToml::saveConfig(const QString &filePath) {
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

    std::stringstream ss;
    ss << m_toml;
    QString tomlStr = QString::fromStdString(ss.str());
    file.write(tomlStr.toUtf8());
    file.close();

    return true;
}

QVariant IToml::getValue(const QString &section, const QString &key) const {
    // Find value in table
    if (auto it = m_toml.find(key.toStdString()); it != m_toml.end()) {
        if (auto strVal = it->second.as_string()) {
            return QString::fromStdString(strVal->get());
        }
    }

    return QString("");
}

void IToml::setValue(const QString &key, const QVariant &value, const QString &section) {
    if (section == "") {
        m_toml.insert_or_assign(key.toStdString(), value);
        // key = value
    } else {
        m_toml.insert_or_assign(section.toStdString(), toml::table{{key.toStdString(), value}});
        // [section]
        // key = value
    }
}

bool IToml::removeValue(const QString &section, const QString &key) {
    auto it = m_toml.find(section.toStdString());
    if (it != m_toml.end()) {
        it->second.erase(key.toStdString());
        return true;
    }
    return false;
}
