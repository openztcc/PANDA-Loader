#ifndef PMODQUERIES_H
#define PMODQUERIES_H

#include <QString>

namespace PQueries {
    // Table Names
    const QString ModsTable = "mods";
    const QString DependenciesTable = "dependencies";

// ----------------------------------- CREATE TABLES -----------------------------------

    const QString CreateModsTable = 
        "CREATE TABLE IF NOT EXISTS mods ("
        "pk INTEGER PRIMARY KEY AUTOINCREMENT, "

        // Mod Properties
        "title TEXT NOT NULL, "
        "authors TEXT NOT NULL, "
        "description TEXT, "
        "version TEXT NOT NULL, "
        "mod_id TEXT NOT NULL UNIQUE, "
        "link TEXT, "

        // Flags
        "enabled INTEGER NOT NULL, "
        "is_selected INTEGER NOT NULL, "
        "listed INTEGER NOT NULL, "
        "is_collection INTEGER NOT NULL, "

        // Categorization
        "category TEXT, "
        "tags TEXT, "

        // File data
        "filename TEXT, "
        "current_location TEXT, "
        "original_location TEXT, "
        "disabled_location TEXT, "
        "file_size TEXT, "
        "file_date TEXT, "

        // External data
        "dep_id TEXT, "
        "collection_id TEXT, "

        // Graphics
        "icon_paths TEXT, "
        "FOREIGN KEY(mod_id) REFERENCES dependencies(mod_id)"
        ");";

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

// ----------------------------------- INSERT QUERIES -----------------------------------

    const QString ModsInsertQuery = 
        "INSERT INTO mods ("
        // Mod Properties
        "title, "
        "authors, "
        "description, "
        "version, "
        "mod_id, "
        "link, "

        // Mod flags
        "enabled, "
        "is_collection, "
        "is_selected, "
        "listed, "

        // Categorization properties
        "category, "
        "tags, "

        // File data
        "filename, "
        "current_location, "
        "original_location, "
        "disabled_location, "
        "file_size, "
        "file_date, "

        // External properties
        "dep_id, "
        "collection_id, "

        // Graphics properties
        "icon_paths "
        ") "

        "VALUES ("

        // Mod Properties
        ":title, "
        ":authors, "
        ":description, "
        ":version, "
        ":mod_id, "
        ":link, "

        // Mod flags
        ":enabled, "
        ":is_collection, "
        ":is_selected, "
        ":listed, "

        // Categorization properties
        ":category, "
        ":tags, "

        // File data
        ":filename, "
        ":current_location, "
        ":original_location, "
        ":disabled_location, "
        ":file_size, "
        ":file_date, "

        // External properties
        ":dep_id, "
        ":collection_id, "

        // Graphics properties
        ":icon_paths "
        ")";

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
}
#endif // PMODQUERIES_H