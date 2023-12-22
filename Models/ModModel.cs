/*
    * ModModel.cs
    * -------------
    * Defines the ModModel class, which is used to store information about a mod
    * in the local Panda database.
*/

public class ModModel
{
    public int Id { get; set; } // Primary Key
    public string Name { get; set; } // Name of the mod
    public bool Status { get; set; } // Is the mod enabled or disabled
    public string Category { get; set; } // Category of the mod
}