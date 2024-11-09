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
        private bool _resolution;
        [ObservableProperty]
        private bool _updateRate;
        [ObservableProperty]
        private bool _drawRate;
        [ObservableProperty]

        // User Settings Section
        private bool _lastFile;
        private bool _unlockEntity0;
        private bool _unlockCount;
        private bool _unlockEntity1;
        private bool _unlockEntity2;
        private bool _showUserEntityWarning;

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
