#include "PEntityType.h"

// Loads the entity type from the entity config
// TODO: eventually only add fields that are needed
std::unique_ptr<PEntityType> PEntityType::load(const QSettings& settings) {
    auto entityType = std::make_unique<PEntityType>();

    // for now, only care about the id and aniPath
    entityType->id = settings.value("id").toString();
    if (PEntityType)
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