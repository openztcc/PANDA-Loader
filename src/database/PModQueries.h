#ifndef PMODQUERIES_H
#define PMODQUERIES_H

#include <QString>

namespace PQueries {
    // Table Names
    const QString ModsTable = "mods";
    const QString DependenciesTable = "dependencies";

    const QString CreateModsTable = 
        "CREATE TABLE IF NOT EXISTS mods ("
        "pk INTEGER PRIMARY KEY AUTOINCREMENT, "
        "title TEXT NOT NULL, "
        "authors TEXT NOT NULL, "
        "description TEXT, "
        "enabled INTEGER NOT NULL, "
        "category TEXT, "
        "tags TEXT, "
        "version TEXT NOT NULL, "
        "mod_id TEXT NOT NULL UNIQUE, "
        "dep_id TEXT, "
        "iconpaths TEXT, "
        "filename TEXT, "
        "location TEXT, "
        "oglocation TEXT, "
        "is_selected INTEGER NOT NULL, "
        "FOREIGN KEY(mod_id) REFERENCES dependencies(mod_id)"
        ");";

    const QString ModsInsertQuery = 
        "INSERT INTO mods ("
        "title, "
        "authors, "
        "description, "
        "enabled, "
        "category, "
        "tags, "
        "version, "
        "mod_id, "
        "dep_id, "
        "filename, "
        "location, "
        "iconpaths, "
        "oglocation, "
        "is_selected) "
        "VALUES ("
        ":title, "
        ":authors, "
        ":description, "
        ":enabled, "
        ":category, "
        ":tags, "
        ":version, "
        ":mod_id, "
        ":dep_id, "
        ":filename, "
        ":location, "
        ":iconpaths, "
        ":oglocation, "
        ":is_selected)";

    const QString DependencyInsertQuery = 
        "INSERT INTO dependencies ("
        "mod_id, "
        "dependency_id, "
        "name, "
        "min_version, "
        "optional, "
        "ordering, "
        "link) "
        "VALUES ("
        ":mod_id, "
        ":dependency_id, "
        ":name, "
        ":min_version, "
        ":optional, "
        ":ordering, "
        ":link)";

    const QString CreateDependenciesTable =
        "CREATE TABLE IF NOT EXISTS dependencies ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "mod_id TEXT NOT NULL, "
        "dependency_id TEXT NOT NULL, "
        "name TEXT NOT NULL, "
        "min_version TEXT, "
        "optional INTEGER NOT NULL, "
        "ordering TEXT, "
        "link TEXT, "
        "FOREIGN KEY(mod_id) REFERENCES mods(mod_id)"
        ");";

}

#endif // PMODQUERIES_H