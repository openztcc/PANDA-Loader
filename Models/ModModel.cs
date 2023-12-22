/*
    * ModModel.cs
    * -------------
    * Defines the ModModel class, which is used to store information about a mod
    * in the local Panda database.
*/

using System;

public class ModModel
{
    public int Id { get; set; } // Primary Key
    public string Name { get; set; } // Name of the mod
    public string Author { get; set; } // Author of the mod
    public bool Status { get; set; } // Is the mod enabled or disabled
    public string Category { get; set; } // Category of the mod
    public string FileLocation { get; set; } // Location of the mod file
    public string Tags { get; set; } // Tags associated with the mod
    public String Notes { get; set; } // Notes associated with the mod
    public bool IsMissing { get; set; } // Is the mod missing
    public String FileName { get; set; } // Name of the mod file
}