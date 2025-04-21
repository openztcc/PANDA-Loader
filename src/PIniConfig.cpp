#include  "PIniConfig.h"

bool PIniConfig::loadConfig(const QString &filePath) {
    SI_Error rc = m_ini.LoadFile(filePath.toStdString().c_str());
    if (rc < 0) {
        qDebug() << "Failed to load INI file: " << filePath;
        return false;
    }
    return true;
}

bool PIniConfig::saveConfig(const QString &filePath) {
    SI_Error rc = m_ini.SaveFile(filePath.toStdString().c_str());
    if (rc < 0) {
        qDebug() << "Failed to save INI file: " << filePath;
        return false;
    }
    return true;
}

QVariant PIniConfig::getValue(const QString &section, const QString &key) const {
    const char *value = m_ini.GetValue(section.toStdString().c_str(), key.toStdString().c_str(), nullptr);
    if (value) {
        return QString::fromStdString(value);
    }
    return QVariant();
}

void PIniConfig::setValue(const QString &key, const QVariant &value, const QString &section) {
    if (section == "") {
        qDebug() << "Error: No section provided for ini file with key = " << key << " and value " << value;
        return;
    }

    m_ini.SetValue(section.toStdString().c_str(), key.toStdString().c_str(), value.toString().toStdString().c_str());
}

bool PIniConfig::removeKey(const QString &section, const QString &key) {
    bool deleteSectionIfEmpty = true;
    m_ini.Delete(section.toStdString().c_str(), key.toStdString().c_str(), deleteSectionIfEmpty);
    return true;
}

bool PIniConfig::removeSection(const QString &section) {
    return false;
}

bool PIniConfig::getAllSections() {
    return false;
}

// Removes all keys given a section and a value
void PIniConfig::removeKeysByValue(const QString &section, const QString &value) {
    // get all the keys in the section
    CSimpleIniA::TNamesDepend keys;
    m_ini.GetAllKeys(section.toStdString().c_str(), keys);

    for (const auto &key : keys) {
        // check if the key is empty
        if (key.pItem == value.toStdString().c_str()) {
            // remove the key from the settings
            bool deleteSectionIfEmpty = true;
            m_ini.Delete(section.toStdString().c_str(), key.pItem, deleteSectionIfEmpty);
        }
    }
}


// --------------- Operator overloads

PIniConfig& PIniConfig::operator=(const PIniConfig& other) {
    if (this == &other) {
        return *this; // self-assignment check
    }

    // // copy the ini from the other ini
    m_ini.Reset();
    m_ini.SetUnicode(other.m_ini.IsUnicode());

    // copy all sections and keys
    CSImpleIniA::TNamesDepend sections;
    other.m_ini.GetAllSections(sections);
    for (const auto& section : sections) {
        CSimpleIniA::TNamesDepend keys;
        other.m_ini.GetAllKeys(section.pItem, keys);
        for (const auto& key : keys) {
            const char* value = other.m_ini.GetValue(section.pItem, key.pItem, nullptr);
            if (value) {
                m_ini.SetValue(section.pItem, key.pItem, value);
            }
        }
    }
    return *this;
}