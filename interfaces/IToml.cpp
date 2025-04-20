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
        IToml::interpretVariant(m_toml, k, value);
        // key = value
    } else {
        // check to make sure section exists
        if (!m_toml.contains(s) || !m_toml[s].is_table()) {
            m_toml.insert_or_assign(s, toml::table{});
        }

        // get ref
        auto& fresh_section = *m_toml[s].as_table();

        IToml::interpretVariant(fresh_section, k, value);
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

// ------------------- HELPERS (to deal with unpredictable types in toml files)

// Interpret a QVariant to std::any so input in UI can be used directly
void IToml::interpretVariant(toml::table& config, const std::string& key, const QVariant& value) {
    switch (value.typeId()) {
    case QMetaType::Bool:
        config.insert_or_assign(key, value.toBool());
        break;
    case QMetaType::Int:
    case QMetaType::Long:
    case QMetaType::LongLong:
    case QMetaType::UInt:
    case QMetaType::ULong:
    case QMetaType::ULongLong:
        config.insert_or_assign(key, static_cast<int64_t>(value.toLongLong()));
        break;
    case QMetaType::Double:
        config.insert_or_assign(key, value.toDouble());
        break;
    case QMetaType::QString:
        config.insert_or_assign(key, value.toString().toStdString());
        break;
    case QMetaType::QVariantList: {
        toml::array arr;
        for (const auto& v : value.toList()) {
            appendVariantToArray(arr, v);
        }
        config.insert_or_assign(key, arr);
        break;
    }
    default:
        break;
    }
}

void IToml::appendVariantToArray(toml::array& arr, const QVariant& value) {
    switch (value.typeId()) {
    case QMetaType::Bool:
        arr.push_back(value.toBool());
        break;
    case QMetaType::Int:
    case QMetaType::Long:
    case QMetaType::LongLong:
    case QMetaType::UInt:
    case QMetaType::ULong:
    case QMetaType::ULongLong:
        arr.push_back(static_cast<int64_t>(value.toLongLong()));
        break;
    case QMetaType::Double:
        arr.push_back(value.toDouble());
        break;
    case QMetaType::QString:
        arr.push_back(value.toString().toStdString());
        break;
    case QMetaType::QVariantList: {
        toml::array nestedArr;
        for (const auto& v : value.toList()) {
            appendVariantToArray(nestedArr, v);
        }
        arr.push_back(nestedArr);
        break;
    }
    default:
        break;
    }
}
