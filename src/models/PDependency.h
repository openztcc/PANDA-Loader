#ifndef PDEPENDENCY_H
#define PDEPENDENCY_H

// Qt
#include <QString>

struct PDependency
{
    QString dependencyId;
    QString modId;
    QString name;
    QString min_version;
    bool optional;
    QString ordering;
    QString link;
};

#endif // PDEPENDENCY_H