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
    std::string k = key.toStdString();

    // Find value in table
    if (auto it = m_toml.find(k); it != m_toml.end()) {
        if (auto strVal = it->second.as_string()) {
            return QString::fromStdString(strVal->get());
        }
    }

    return QString("");
}

void IToml::setValue(const QString &key, const QVariant &value, const QString &section) {
    std::string k = key.toStdString();
    std::string s = section.toStdString();

    if (section == "") {
        m_toml.insert_or_assign(k, value);
        // key = value
    } else {
        m_toml.insert_or_assign(s, toml::table{{k, value}});
        // [section]
        // key = value
    }
}

bool IToml::removeKey(const QString &key, const QString &section) {
    std::string k = key.toStdString();
    std::string s = section.toStdString();

    if (s == "") {
        m_toml.erase(k);
        return true;
    } else {
        if (auto* settings = m_toml[s].as_table()) {
            settings->erase(k);
            return true;
        }
    }

    return false;
}

bool IToml::removeSection(const QString &section) {
    std::string s = section.toStdString();

    if (m_toml.contains(s)) {
        m_toml.erase(s);
        return true;
    } else {
        return false;
    }
}
