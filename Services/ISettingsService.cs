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
        void SaveIni(IniData data);
        IniData LoadIni();
    }

    public class IniService : ISettingsService
    {
        private readonly FileIniDataParser _parser;
        private readonly string _path;

        public IniService(string path)
        {
            _parser = new FileIniDataParser();
            _path = path;
        }

        public IniData LoadIni()
        {
            return _parser.ReadFile(_path);
        }

        public void SaveIni(IniData data)
        {
            _parser.WriteFile(_path, data);
        }
    }
}
