#ifndef PENTITYTYPE_H
#define PENTITYTYPE_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <memory>
#include "../models/PIconData.h"
#include <QSettings>
#include "../src/PConfigMgr.h"

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
    QList ztdPath;
    QMap<QString, QString> characteristics;
    QMap<QString, QString> iconAniPaths;

    std::unique_ptr<PEntityType> load(const QSettings& settings, const QString& path);
    static Type getType(const QString& path);

    void loadAniPaths(QStringList& iconPaths);
    void loadIconPath(QString& iconPath);
    PEntityType();

    // helper functions
    static QString determineViewFromPath(const QString& aniPath);
}

#endif // PENTITYTYPE_H