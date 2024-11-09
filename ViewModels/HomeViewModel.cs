using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using PandaLdr.Models;
using System;

namespace PandaLdr.ViewModels
{
    public partial class HomeViewModel : ViewModelBase
    {
        private string _modPath = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\dlupdate";
        private string _mapPath = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\maps";

        [ObservableProperty]
        private int _installedModCount;
        [ObservableProperty]
        private int _installedMapCount;
        [ObservableProperty]
        private string _pandaVersion;
        [ObservableProperty]
        private string _oztVersion = "0.1.0";

        // INI Configuration

        // Display Settings
        [ObservableProperty]
        private bool _fullScreen;
        [ObservableProperty]
        private string _resolution;
        [ObservableProperty]
        private string _updateRate;
        [ObservableProperty]
        private string _drawRate;
        [ObservableProperty]

        // User Settings Section
        private string _lastFile;
        private string _unlockEntity0;
        private string _unlockCount;
        private string _unlockEntity1;
        private string _unlockEntity2;
        private bool _showUserEntityWarning;

        // Advanced Settings Section
        private string _drag;
        private string _click;
        private string _normal;
        private string _level;
        private bool _loadHalfAnimations;
        private bool _use8BitSound;
        private bool _developerModeEnabled;


        public HomeViewModel()
        {
            InstalledModCount = ArchiveMgr.GetArchives(_modPath).Count;
            Console.WriteLine("Installed mods: " + InstalledModCount);

            InstalledMapCount = MapMgr.GetMaps(_mapPath).Count;
            Console.WriteLine("Installed maps: " + InstalledMapCount);

            PandaVersion = "0.1.0";
        }
    }
}
