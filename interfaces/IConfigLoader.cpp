#include "IConfigLoader.h"

// Interpret a QVariant to std::any so input in UI can be used directly
// TODO: update if 32 bit support is added
std::optional<std::any> IConfigLoader::interpretVariant(const QVariant &value) {
    switch (value.typeId()) {
    case QMetaType::Bool:
        return value.toBool();
    case QMetaType::Int:
    case QMetaType::Long:
    case QMetaType::LongLong:
    case QMetaType::UInt:
    case QMetaType::ULong:
    case QMetaType::ULongLong:
        return static_cast<int64_t>(value.toLongLong());
    case QMetaType::Double:
        return value.toDouble();
    case QMetaType::QString:
        return value.toString().toStdString();
    case QMetaType::QVariantList: {
        std::vector<std::any> list;
        for (const auto& v : value.toList()) {
            if (auto converted = variantToAny(v))
                list.push_back(*converted);
        }
        return list;
    }
    default:
        return std::nullopt;
    }
}