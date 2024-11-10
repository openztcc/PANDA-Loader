using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using IniParser;
using IniParser.Model;

namespace PandaLdr.Services
{
    using IniParser.Model;

    public interface ISettingsService
    {
        void SaveZooINI(string path, IniData data);
        IniData LoadZooINI(string path);
    }
}
