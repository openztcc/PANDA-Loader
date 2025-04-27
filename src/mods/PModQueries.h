#ifndef PMODQUERIES_H
#define PMODQUERIES_H

#include <QString>

namespace PQueries {
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
}

#endif // PMODQUERIES_H