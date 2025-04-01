#include "PEntityType.h"

// Loads the entity type from the entity config
// TODO: eventually expand to load all config properties depending on the type
std::unique_ptr<PEntityType> PEntityType::load(const QSettings& settings, const QString& path) {
    auto entityType = std::make_unique<PEntityType>();

    // for now, only care about the id and aniPath
    if (PEntityType::getType(path) == Type::Animal) {
        settings.beginGroup("Global");
        entityType->id = settings.value("Type").toString();
        settings.endGroup();

        // debugging
        if (entityType->id.isEmpty()) {
            qDebug() << "Entity type id is empty for path:" << path;
        }

        // can be one of two paths under two sections: [m/Icon] or [f/Icon]
        // get Type from first section if exists
        settings.beginGroup("m/Icon");
        if (settings.contains("Type")) {
            QString aniPath = settings.value("Type").toString();
            if (!aniPath.isEmpty()) {
                entityType->iconAniPaths.push_back({entityType->id, aniPath, "", false});
            } else {
                qDebug() << "m/Icon .ani path is empty for:" << path;
            }
        }
        settings.endGroup();
        // get Type from second section if exists
        settings.beginGroup("f/Icon");
        if (settings.contains("Type")) {
            QString aniPath = settings.value("Type").toString();
            if (!aniPath.isEmpty()) {
                entityType->iconAniPaths.push_back({entityType->id, aniPath, "", false});
            } else {
                qDebug() << "f/Icon .ani path is empty for:" << path;
            }
        }
        settings.endGroup();
        
    } else if (PEntityType::getType(path) == Type::Scenery) {
        // For any scenery type, we need to find the [Icon] section
        // these have four identical keys named Icon so can't parse 
        settings.beginGroup("Icon");
        // get group as a list of strings
        QStringList iconPaths = settings.allKeys();

        // get all "Icon" keys
        for (const QString& iconPath : iconPaths) {
            if (iconPath.compare("Icon", Qt::CaseInsensitive) == 0) {
                // get the ani path
                QString aniPath = settings.value(iconPath).toString();
                QVariant value = settings.value(iconPath);
                
                if (value.type() == QVariant::StringList) {
                    QStringList aniPaths = value.toStringList();
                    for (const QString& aniPath : aniPaths) {
                        if (!aniPath.isEmpty()) {
                            bool pathsFinalized = false;

                            if (aniPath.contains(".tga")) {
                                pathsFinalized = true;
                            }

                            // determine view type from the ani path
                            QString view = PEntityType::determineViewFromPath(aniPath);
                            // add the ani path to the list of icon ani paths
                            entityType->iconAniPaths.push_back({view, aniPath, "", pathsFinalized});

                        } else {
                            qDebug() << "Icon .ani path is empty for:" << path;
                        }
                    }
                } else {
                    // if it's not a list, just add the ani path
                    if (!aniPath.isEmpty()) {
                        bool pathsFinalized = false;

                        if (aniPath.contains(".tga")) {
                            pathsFinalized = true;
                        }

                        // determine view type from the ani path
                        QString view = PEntityType::determineViewFromPath(aniPath);
                        // add the ani path to the list of icon ani paths
                        entityType->iconAniPaths.push_back({view, aniPath, "", pathsFinalized});
                    } else {
                        qDebug() << "Icon .ani path is empty for:" << path;
                    }
                }
            } 
        }

    }

    // null initialization for other fields
    entityType->characteristics.clear();

    return entityType;
}

// Gets the type of the entity based on the path
// TODO: eventually add more specific checks for each type
// Initial purpose is to find out what kind of ani paths to load
// Note: Food types will require parsing [Characteristics/Integers] for the  cFood = 1 property
// - no apparent MemberType for food. Another characteristic is they do not have a [Member] section.
// - Large poop also not a member type, but can be identified with cNameID = 6062 under [Characteristics/Integers]
//   Small zoo doo: cNameID = 6093
// - Dino poo: cNameID = 6094
// - Trash: cNameID = 6310
// - Arch: cNameID = 6020
// - Rubble: cNameID = 10513 - also cRock = 1
// ZTUnits don't have a member type but have [Global] section with Class = Type
// - Class = staff - Keeper, Maint, Tour, Helicopter, Scientist
// - Class = animals - For animals, Type usually is the entity id as well
Type PEntityType::getType (const QString& path) {
    if (path.contains("animal")) {
        return Type::Animal;
    } else if (path.contains("scenery")) {
        // scenery can mean several types
        // TODO: add more specific checks for scenery types
        // might need to go into the config
        return Type::Scenery;
    }

    return Type::Unknown;
}

// Determine direction of icon from ani path
QString PEntityType::determineViewFromPath(const QString& aniPath) {
    if (aniPath.contains("NE")){
        return "NE";
    } else if (aniPath.contains("NW")) {
        return "NW";
    } else if (aniPath.contains("SE")) {
        return "SE";
    } else if (aniPath.contains("SW")) {
        return "SW";
    } else {
        return "NA";
    }
}