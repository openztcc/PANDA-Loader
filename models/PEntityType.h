#ifndef PENTITYTYPE_H
#define PENTITYTYPE_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <memory>
#include "../models/PIconData.h"

class PEntityType {
public:
    // Supported types
    enum Type {
        Animal,
        Scenery,
        Building,
        AnimalFood,
        TankFilter,
        Fence,
        TankWall
        Path,
        Rubble,
        Unit,
        Guest,
        Staff,
        MaintenanceWorker,
        Helicopter,
        Guide,
        Keeper,
        Ambient,
        Unknown
    };

    enum MemberTypes {
        Entity,
        Unit,
        ZTUnit,
        Animals,
        Scenery,
        Building,
        Staff,
        Ambient,
        ZooFences,
        LowFence,
        Fence,
        HabitatFences,
        HighFence,
        Developer,
        ZooGate,
        Foliage,
        Shelters,
        Rocks,
        Trash,
        Poo,
        Structures,
        Toys,
        HabitatFoliage,
        ZooFoliage,
        Dinosaur,
        All,
        Aqua,
        ShowToys,
    };

    enum ClassType {
        Keeper,
        Maint,
        Tour,
        Helicopter,
        Scient,
        Trainer
    }

    QString id;
    QLIst<PIconData> icons;
    QMap<QString, QString> characteristics;
    QMap<QString, QString> animPaths;

    static std::unique_ptr<PEntityType> load(const QSettings& settings);
    static Type getType(const QString& path);
    PEntityType();
}

#endif // PENTITYTYPE_H