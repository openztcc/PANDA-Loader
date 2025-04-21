#include  "PIniConfig.h"

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
    if (section == "") {
        qDebug() << "Error: No section provided for ini file with key = " << key << " and value " << value;
        return;
    }

    m_ini.SetValue(section.toStdString().c_str(), key.toStdString().c_str(), value.toString().toStdString().c_str());
}

bool IIni::removeKey(const QString &section, const QString &key) {
    bool deleteSectionIfEmpty = true;
    m_ini.Delete(section.toStdString().c_str(), key.toStdString().c_str(), deleteSectionIfEmpty);
    return true;
}

bool IIni::removeSection(const QString &section) {
    return false;
}

bool IIni::getAllSections() {
    return false;
}

// Removes all keys given a section and a value
void IIni::removeKeysByValue(const QString &section, const QString &value) {
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
