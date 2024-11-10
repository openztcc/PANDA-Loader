using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using IniParser;
using IniParser.Model;

namespace PandaLdr.Models
{
    class INIMgr
    {
        public static void SaveZooINI(string path, IniData data)
        {
            // Save the data to the Zoo.ini file
            FileIniDataParser parser = new FileIniDataParser();
            parser.WriteFile(path, data);
        }

    }
}
