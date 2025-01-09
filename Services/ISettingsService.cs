using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using IniParser;
using IniParser.Model;

/* 
    IServicesService.cs

    Defines an interface that allows for the loading and saving of INI files.
*/

namespace PandaLdr.Services
{
    using IniParser.Model;

    public interface ISettingsService // Interface for loading and saving INI files
    {
        void SaveIni(IniData data);
        IniData LoadIni();
    }

    public class IniService : ISettingsService
    {
        private readonly FileIniDataParser _parser; // INI parser
        private readonly string _path; // Path to the INI file

        public IniService(string path) // Constructor
        {
            _parser = new FileIniDataParser();
            _path = path;
        }

        public IniData LoadIni() // Load the INI file
        {
            return _parser.ReadFile(_path);
        }

        public void SaveIni(IniData data) // Save the INI file
        {
            _parser.WriteFile(_path, data);
        }
    }
}
