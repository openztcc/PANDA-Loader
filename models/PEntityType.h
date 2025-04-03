#ifndef PENTITYTYPE_H
#define PENTITYTYPE_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <memory>
#include "../models/PIconData.h"
#include <QSettings>

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
        TankWall,
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
        UnitMember,
        ZTUnit,
        Animals,
        SceneryMember,
        BuildingMember,
        StaffMember,
        AmbientMember,
        ZooFences,
        LowFence,
        FenceMember,
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
        KeeperClass,
        Maint,
        Tour,
        HelicopterClass,
        Scient,
        Trainer
    };

    QString id;
    QList<PIconData> icons;
    QString ztdPath;
    QMap<QString, QString> characteristics;
    QMap<QString, QString> iconAniPaths;

    void load(QSettings& settings, const QString& path);
    static Type getType(const QString& path);

    void loadAniPaths(QStringList& iconPaths);
    void loadIconPath(QString& iconPath);
    PEntityType();

    // helper functions
    static QString determineViewFromPath(const QString& aniPath);
};

#endif // PENTITYTYPE_H
