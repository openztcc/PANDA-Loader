using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data.SQLite;

namespace PandaLdr.Models
{
    class ArchiveMgr
    {
        public static List<string> GetArchives(string path)
        {
            // Get a list of all files in the path directory
            string[] files = System.IO.Directory.GetFiles(path);
            List<string> archives = new List<string>();

            // Add all .ztd files to the list
            foreach (string file in files)
            {
                if (file.EndsWith(".ztd"))
                {
                    archives.Add(file);
                }
            }

            return archives;
        }

        public static void StoreArchives(List<string> archives)
        {
            // Store the list of archives in a SQLite database
            using (SQLiteConnection m_dbConnection = new SQLiteConnection("Data Source=PandaConsole2.db;Version=3;"))
            {
                try
                {
                    m_dbConnection.Open();
                    CreateTable(m_dbConnection, "Archives");
                    foreach (string archive in archives)
                    {
                        InsertData(m_dbConnection, archive, "Archives");
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine("An error occurred saving files to DB: " + ex.Message);
                }
            }
        }

        public static void PrintArchives(List<string> archives)
        {
            //foreach (string archive in archives)
            //{
            //    string file_name = System.IO.Path.GetFileName(archive);
            //    if (file_name.EndsWith(".ztd"))
            //    {
            //        Console.WriteLine("[" + archives.IndexOf(archive) + "] " + file_name);
            //    }
            //}

            // Retrieve items from the database
            using (SQLiteConnection m_dbConnection = new SQLiteConnection("Data Source=PandaConsole2.db;Version=3;"))
            {
                m_dbConnection.Open();
                RetrieveItems(m_dbConnection, "Archives");
            }
        }

        public static void DeleteArchive(int index, List<string> archives)
        {
            string dir = archives[index];
            string file_name = System.IO.Path.GetFileName(dir);
            Console.WriteLine("PANDA will delete " + file_name + ". Are you sure? (y/n)");
            string answer = Console.ReadLine();
            if (answer == "y")
            {
                System.IO.File.Delete(dir);
                archives.RemoveAt(index);
                // Delete the archive from the database
                using (SQLiteConnection m_dbConnection = new SQLiteConnection("Data Source=PandaConsole2.db;Version=3;"))
                {
                    m_dbConnection.Open();
                    string sql = "DELETE FROM Archives WHERE Item = @archive";
                    using (SQLiteCommand command = new SQLiteCommand(sql, m_dbConnection))
                    {
                        command.Parameters.AddWithValue("@archive", dir);
                        command.ExecuteNonQuery();
                    }
                }

                Console.WriteLine(file_name + " has been deleted.");
                return;
            }
            Console.WriteLine(file_name + " was not deleted.");
        }

        // Save favorite archives to a file
        public void FavoriteArchives(List<string> archives)
        {
            string path = @"C:\Program Files (x86)\Microsoft Games\Zoo Tycoon\1028\halloween\favorite_archives.txt";
            using (System.IO.StreamWriter file = new System.IO.StreamWriter(path))
            {
                foreach (string archive in archives)
                {
                    file.WriteLine(archive);
                }
            }

        }

        // Insert data into a given table in the database
        public static void InsertData(SQLiteConnection m_dbConnection, string archive, string table)
        {
            // Define the SQL command with a parameter placeholder
            string sql = "INSERT INTO " + table + " (Item) VALUES (@archive)";

            // Create the SQLiteCommand
            using (SQLiteCommand command = new SQLiteCommand(sql, m_dbConnection))
            {
                // Add the parameter value
                command.Parameters.AddWithValue("@archive", archive);

                // Execute the command
                command.ExecuteNonQuery();
            }
        }


        // This method is used to create a new table in the database
        static void CreateTable(SQLiteConnection m_dbConnection, string table)
        {
            try
            {
                // Ensure the connection is open
                if (m_dbConnection.State != System.Data.ConnectionState.Open)
                {
                    m_dbConnection.Open();
                }

                // Validate the table name
                if (string.IsNullOrWhiteSpace(table) || table.IndexOfAny(" ;'\"".ToCharArray()) != -1)
                {
                    throw new ArgumentException("Invalid table name");
                }

                // Define the SQL command
                string sql = "CREATE TABLE IF NOT EXISTS [" + table + "] (Id INTEGER PRIMARY KEY, Item TEXT);";

                // Create the SQLiteCommand
                using (SQLiteCommand command = new SQLiteCommand(sql, m_dbConnection))
                {
                    // Execute the command
                    command.ExecuteNonQuery();
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("An error occurred: " + ex.Message);
                // Handle the exception appropriately (e.g., log it, rethrow it, etc.)
            }
        }


        // Retrieve items from a given table in the database
        static void RetrieveItems(SQLiteConnection connection, string table)
        {
            try
            {
                // Ensure the connection is open
                if (connection.State != System.Data.ConnectionState.Open)
                {
                    connection.Open();
                }

                // Validate the table name
                if (string.IsNullOrWhiteSpace(table) || table.IndexOfAny(" ;'\"".ToCharArray()) != -1)
                {
                    throw new ArgumentException("Invalid table name");
                }

                // Define the SQL command
                string sql = "SELECT * FROM [" + table + "];";

                // Create the SQLiteCommand
                using (SQLiteCommand command = new SQLiteCommand(sql, connection))
                {
                    // Execute the command
                    using (SQLiteDataReader reader = command.ExecuteReader())
                    {
                        // Read the data and display it
                        while (reader.Read())
                        {
                            // print the item with the index (Id)
                            Console.WriteLine("[" + reader["Id"] + "] " + reader["Item"]);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("An error occurred: " + ex.Message);
                // Handle the exception appropriately (e.g., log it, rethrow it, etc.)
            }
        }


        public void RegisterArchive(string path)
        {

        }
    }
}
