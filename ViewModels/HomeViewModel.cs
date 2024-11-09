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

        // User Settings Section
        [ObservableProperty]
        private string _lastFile;
        [ObservableProperty]
        private string _unlockEntity0;
        [ObservableProperty]
        private string _unlockCount;
        [ObservableProperty]
        private string _unlockEntity1;
        [ObservableProperty]
        private string _unlockEntity2;
        [ObservableProperty]
        private bool _showUserEntityWarning;

        // Advanced Settings Section
        [ObservableProperty]
        private string _drag;
        [ObservableProperty]
        private string _click;
        [ObservableProperty]
        private string _normal;
        [ObservableProperty]
        private string _level;
        [ObservableProperty]
        private bool _loadHalfAnimations;
        [ObservableProperty]
        private bool _use8BitSound;
        [ObservableProperty]
        private bool _developerModeEnabled;

        // UI Settings Section
        [ObservableProperty]
        private bool _useAlternateCursors;
        [ObservableProperty]
        private string _tooltipDelay;
        [ObservableProperty]
        private string _tooltipDuration;
        [ObservableProperty]
        private string _messageDisplay;
        [ObservableProperty]
        private string _mouseScrollThreshold;
        [ObservableProperty]
        private string _mouseScrollDelay;
        [ObservableProperty]
        private string _mouseScrollX;
        [ObservableProperty]
        private string _mouseScrollY;
        [ObservableProperty]
        private string _keyScrollX;
        [ObservableProperty]
        private string _keyScrollY;
        [ObservableProperty]
        private string _minimumMessageInterval;
        [ObservableProperty]
        private string _defaultEditCharLimit;
        [ObservableProperty]
        private bool _noMenuMusic;  
        [ObservableProperty]
        private string _menuMusic;
        [ObservableProperty]
        private string _helpType;
        [ObservableProperty]
        private bool _playMovie;
        [ObservableProperty]
        private string _movieVolume1;
        [ObservableProperty]
        private bool _playSecondMovie;
        [ObservableProperty]
        private string _movieVolume2;
        [ObservableProperty]
        private string _maxShortTooltipWidth;
        [ObservableProperty]
        private string _maxLongTooltipWidth;
        [ObservableProperty]
        private string _progressLeft;
        [ObservableProperty]
        private string _progressTop;
        [ObservableProperty]
        private string _progressRight;
        [ObservableProperty]
        private string _progressBottom;
        [ObservableProperty]
        private string _progressRed;
        [ObservableProperty]
        private string _progressGreen;
        [ObservableProperty]
        private string _progressBlue;
        [ObservableProperty]
        private string _progressShadowXOffset;
        [ObservableProperty]
        private string _progressShadowYOffset;
        [ObservableProperty]
        private string _progressShadowRed;
        [ObservableProperty]
        private string _progressShadowGreen;
        [ObservableProperty]
        private string _progressShadowBlue;
        [ObservableProperty]
        private string _completedExhibitAttenuation;
        [ObservableProperty]
        private string _msStartingCash;
        [ObservableProperty]
        private string _msCashIncrement;
        [ObservableProperty]
        private string _msMinCash;
        [ObservableProperty]
        private string _msMaxCash;
        [ObservableProperty]
        private bool _startedFirstTutorial;
        [ObservableProperty]
        private bool _startedDinoTutorial;
        [ObservableProperty]
        private bool _startedAquaTutorial;
        [ObservableProperty]
        private string _lastWindowX;
        [ObservableProperty]
        private string _lastWindowY;
        [ObservableProperty]
        private string _progressCalls;

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
