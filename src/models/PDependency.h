#ifndef PDEPENDENCY_H
#define PDEPENDENCY_H

// Qt
#include <QString>

struct PDependency
{
    QString dependencyId; // id of the dependency should refer to the mod_id of the mod it depends on
    QString modId; // owner of the dependency
    QString name;
    QString min_version;
    bool optional;
    QString ordering;
    QString link;
};

#endif // PDEPENDENCY_H