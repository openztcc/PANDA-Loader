#include "PTomlConfig.h"

bool PTomlConfig::loadConfig(const QString &filePath) {
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

bool PTomlConfig::saveConfig(const QString &filePath) {
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

bool PTomlConfig::clear() {
    m_toml.clear();
    return true;
}

//TODO: add support for nested tables and arrays
QVariant PTomlConfig::getValue(const QString &section, const QString &key) const {
    std::string k = key.toStdString();
    std::string s = section.toStdString();

    const toml::table* table = &m_toml;

    // if section is not empty, check if it exists and get the table 
    if (!section.isEmpty()) {
        auto found = m_toml.find(s);
        if (found == m_toml.end() || !found->second.is_table()) {
            qDebug() << "Section not found or not a table:" << section;
            return QVariant();
        }
        table = found->second.as_table();
    }

    // check if key exists in the table
    auto it = table->find(k);
    if (k.isEmpty() || it == table->end()) {
        QVariantMap result;
        for (const auto& [mapKey, mapValue] : *table) {
            result.insert(QString::fromStdString(mapKey), extractVariant(mapValue));
        }
        return result;
    }

    // return correct type
    return extractVariant(it->second);
}



void PTomlConfig::setValue(const QString &key, const QVariant &value, const QString &section) {
    std::string k = key.toStdString();
    std::string s = section.toStdString();

    if (section == "") {
        PTomlConfig::interpretVariant(m_toml, k, value);
        // key = value
    } else {
        // check to make sure section exists
        if (!m_toml.contains(s) || !m_toml[s].is_table()) {
            m_toml.insert_or_assign(s, toml::table{});
        }

        // get ref
        auto& fresh_section = *m_toml[s].as_table();

        PTomlConfig::interpretVariant(fresh_section, k, value);
        // [section]
        // key = value
    }
}

bool PTomlConfig::removeKey(const QString &key, const QString &section) {
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

bool PTomlConfig::removeSection(const QString &section) {
    std::string s = section.toStdString();

    if (m_toml.contains(s)) {
        m_toml.erase(s);
        return true;
    } else {
        return false;
    }
}

bool PTomlConfig::getAllSections() {
    return false;
}

// TODO: unit test for this function
QStringList PTomlConfig::getAllKeys(const QString &section) {
    std::string s = section.toStdString();
    QStringList keys;

    if (s == "") {
        for (const auto& [key, value] : m_toml) {
            keys.append(QString::fromStdString(key.data()));
        }
    } else {
        if (auto* settings = m_toml[s].as_table()) {
            for (const auto& [key, value] : *settings) {
                keys.append(QString::fromStdString(key.data()));
            }
        }
    }

    return keys;
}

// ---------------------------- EXIST TESTS

bool PTomlConfig::sectionExists(const QString &section) const {
    // to avoid problem of detecting keys or values bya ccident, test for table first
    auto* isTable = m_toml[section.toStdString()].as_table();
    if (isTable) {
        return true;
    }
    return false;
}

bool PTomlConfig::keyExists(const QString &key, const QString &section) const {
    std::string k = key.toStdString();
    std::string s = section.toStdString();

    if (s == "") {
        return m_toml.contains(k);
    } else {
        if (auto* settings = m_toml[s].as_table()) {
            return settings->contains(k);
        }
    }

    return false;
}

bool PTomlConfig::valueExists(const QString &value, const QString &key, const QString &section) const {
    if (value.isNull()) {
        return false;
    }

    std::string k = key.toStdString();
    std::string s = section.toStdString();
    const std::string expected = value.toStdString();

    const toml::table* table = &m_toml;

    // look through nested tables ("sections") first
    if (!section.isEmpty()) {
        auto found = m_toml.find(s);
        if (found == m_toml.end() || !found->second.is_table()) {
            return false;
        }
        // found a nested table, so get the table ref
        table = found->second.as_table(); 
    }

    // check if key exists in the table
    auto it = table->find(k);
    if (it == table->end()) {
        return false;
    }

    // here we just cast the node to expected type and compare it to the value
    const toml::node& node = it->second;

    if (auto val = node.as_string()) {
        return val->get() == expected;
    } else if (auto val = node.as_integer()) {
        return QString::number(val->get()) == value;
    } else if (auto val = node.as_floating_point()) {
        return QString::number(val->get(), 'f', 6) == value; // precision
    } else if (auto val = node.as_boolean()) {
        return (val->get() ? "true" : "false") == expected;
    }

    return false;
}



// ------------------- HELPERS (to deal with unpredictable types in toml files)

// Interpret a QVariant to std::any so input in UI can be used directly
void PTomlConfig::interpretVariant(toml::table& config, const std::string& key, const QVariant& value) {
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

// same as above but tries to return list
void PTomlConfig::appendVariantToArray(toml::array& arr, const QVariant& value) {
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

// returns as QVariant for flexibility
QVariant PTomlConfig::extractVariant(const toml::node& node) const {
    if (auto val = node.as_string())
        return QString::fromStdString(val->get());
    if (auto val = node.as_boolean())
        return val->get();
    if (auto val = node.as_integer())
        return static_cast<qint64>(val->get());
    if (auto val = node.as_floating_point())
        return val->get();
    if (auto val = node.as_array()) {
        QVariantList list;
        for (const auto& item : *val) {
            list.append(extractVariant(item));
        }
        return list;
    }

    // return QVariant();
    return QVariant(); // interpret as string
}

// returns as QVariant for flexibility
QVariant PTomlConfig::extractVariant(const QString& variant) const {
    // if (auto val = node.as_string())
    //     return QString::fromStdString(val->get());
    // if (auto val = node.as_boolean())
    //     return val->get();
    // if (auto val = node.as_integer())
    //     return static_cast<qint64>(val->get());
    // if (auto val = node.as_floating_point())
    //     return val->get();
    // if (auto val = node.as_array()) {
    //     QVariantList list;
    //     for (const auto& item : *val) {
    //         list.append(extractVariant(item));
    //     }
    //     return list;
    // }

    if (variant.isEmpty()) {
        return QVariant(""); // empty string
    }

    if (variant == "true" || variant == "True") {
        return true; // boolean true
    } else if (variant == "false" || variant == "False") {
        return false; // boolean false
    }

    bool ok = false;
    int intVal = variant.toInt(&ok);
    if (ok) {
        return intVal;
    }

    double dblVal = variant.toFloat(&ok);
    if (ok) {
        return dblVal;
    }

    // return QVariant();
    return QVariant(variant); // interpret as string
}

