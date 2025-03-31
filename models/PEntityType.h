#ifndef PENTITYTYPE_H
#define PENTITYTYPE_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <memory>
#include "../models/PIconData.h"

class EntityType {
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

    QString id;
    bool color;
    bool ncolors;
    bool cIconZoom;
    bool cExpansionID;
    bool cMoveable;
    bool cUseNumbersInName;
    bool cUsesRealShadows;
    bool cHasShadowImages;
    bool cForceShadowBlack;
    bool cDrawsLate;
    int cHeight;
    int cDepth;
    bool cHasUnderwaterSection;
    bool cIsTransparent;
    bool cUsesPlacementCube;
    bool cShow;
    bool cHitThreshold;
    bool fullpal

    EntityType();
    

}

#endif // PENTITYTYPE_H