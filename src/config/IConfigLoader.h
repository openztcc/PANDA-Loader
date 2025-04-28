#ifndef ICONFIGLOADER_H
#define ICONFIGLOADER_H

#include <QtCore>

class IConfigLoader {
    public:
        virtual ~IConfigLoader() = default;
        virtual bool loadConfig(const QString &filePath) = 0;
        virtual bool saveConfig(const QString &filePath) = 0;
        virtual bool clear() = 0;

        virtual QVariant getValue(const QString &section, const QString &key) const = 0;
        virtual void setValue(const QString &key, const QVariant &value, const QString &section) = 0;
        virtual bool removeKey(const QString &section, const QString &key) = 0;
        virtual bool removeSection(const QString &section) = 0;
        virtual bool getAllSections() = 0;
        virtual QStringList getAllKeys(const QString &section) = 0;

        virtual bool sectionExists(const QString &section) const = 0;
        virtual bool keyExists(const QString &key, const QString &section) const = 0;
        virtual bool valueExists(const QString &value, const QString &key, const QString &section) const = 0;

        virtual std::unique_ptr<IConfigLoader> clone() const = 0; // clone so we can use polymorphism
        virtual QVariant extractVariant(const QString& variant) const = 0;
    };

#endif // IConfigLoader.h
