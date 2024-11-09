using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using PandaLdr.Models;
using System;
using IniParser;
using IniParser.Model;

namespace PandaLdr.ViewModels
{
    public partial class HomeViewModel : ViewModelBase
    {
        private string _modPath = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\dlupdate";
        private string _mapPath = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\maps";
        private string _zooIniPath = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\zoo.ini";

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

        // Map Settings Section
        [ObservableProperty]
        private string _mapX;
        [ObservableProperty]
        private string _mapY;

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

        private string[] parseResolution(string resolution)
        {
            string[] res = resolution.Split('x');
            return res;
        }

        public HomeViewModel()
        {
            InstalledModCount = ArchiveMgr.GetArchives(_modPath).Count;
            Console.WriteLine("Installed mods: " + InstalledModCount);

            InstalledMapCount = MapMgr.GetMaps(_mapPath).Count;
            Console.WriteLine("Installed maps: " + InstalledMapCount);

            PandaVersion = "0.1.0";

            // Load INI Configuration

            var parser = new FileIniDataParser();
            IniData data = parser.ReadFile(_zooIniPath);

            // Display Settings
            string FullScreenStr = data["user"]["fullscreen"];
            FullScreen = Convert.ToBoolean(int.Parse(FullScreenStr));
            var screenWidth = data["user"]["screenwidth"];
            var screenHeight = data["user"]["screenheight"];
            Resolution = screenWidth + "x" + screenHeight;
            UpdateRate = data["user"]["UpdateRate"];
            DrawRate = data["user"]["DrawRate"];
            LastFile = data["user"]["lastfile"];
            UnlockEntity0 = data["user"]["unlockEntity0"];
            UnlockCount = data["user"]["unlockCount"];
            UnlockEntity1 = data["user"]["unlockEntity1"];
            UnlockEntity2 = data["user"]["unlockEntity2"];
            string ShowUserEntityWarningStr = data["user"]["showUserEntityWarning"];
            ShowUserEntityWarning = Convert.ToBoolean(int.Parse(ShowUserEntityWarningStr));

            // Advanced Settings
            Drag = data["advanced"]["drag"];
            Click = data["advanced"]["click"];
            Normal = data["advanced"]["normal"];
            Level = data["advanced"]["level"];
            string LoadHalfAnimationsStr = data["advanced"]["loadHalfAnims"];
            LoadHalfAnimations = Convert.ToBoolean(int.Parse(LoadHalfAnimationsStr));
            string Use8BitSoundStr = data["advanced"]["use8BitSound"];
            Use8BitSound = Convert.ToBoolean(int.Parse(Use8BitSoundStr));
            string DeveloperModeEnabledStr = data["advanced"]["devModeEnabled"];
            DeveloperModeEnabled = Convert.ToBoolean(int.Parse(DeveloperModeEnabledStr));

            // Map Settings
            MapX = data["map"]["mapX"];
            MapY = data["map"]["mapY"];

            // UI Settings
            string UseAlternateCursorsStr = data["UI"]["useAlternateCursors"];
            UseAlternateCursors = Convert.ToBoolean(int.Parse(UseAlternateCursorsStr));
            TooltipDelay = data["UI"]["tooltipDelay"];
            TooltipDuration = data["UI"]["tooltipDuration"];
            MessageDisplay = data["UI"]["MessageDisplay"];
            MouseScrollThreshold = data["UI"]["mouseScrollThreshold"];
            MouseScrollDelay = data["UI"]["mouseScrollDelay"];
            MouseScrollX = data["UI"]["mouseScrollX"];
            MouseScrollY = data["UI"]["mouseScrollY"];
            KeyScrollX = data["UI"]["keyScrollX"];
            KeyScrollY = data["UI"]["keyScrollY"];
            MinimumMessageInterval = data["UI"]["minimumMessageInterval"];
            DefaultEditCharLimit = data["UI"]["defaultEditCharLimit"];
            string NoMenuMusicStr = data["UI"]["noMenuMusic"];
            NoMenuMusic = Convert.ToBoolean(int.Parse(NoMenuMusicStr));
            MenuMusic = data["UI"]["menuMusic"];
            HelpType = data["UI"]["helpType"];
            string PlayMovieStr = data["UI"]["playMovie"];
            PlayMovie = Convert.ToBoolean(int.Parse(PlayMovieStr));
            MovieVolume1 = data["UI"]["movievolume1"];
            string PlaySecondMovieStr = data["UI"]["playSecondMovie"];
            PlaySecondMovie = Convert.ToBoolean(int.Parse(PlaySecondMovieStr));
            MovieVolume2 = data["UI"]["movievolume2"];
            MaxShortTooltipWidth = data["UI"]["maxShortTooltipWidth"];
            MaxLongTooltipWidth = data["UI"]["maxLongTooltipWidth"];
            ProgressLeft = data["UI"]["progressLeft"];
            ProgressTop = data["UI"]["progressTop"];
            ProgressRight = data["UI"]["progressRight"];
            ProgressBottom = data["UI"]["progressBottom"];
            ProgressRed = data["UI"]["progressRed"];
            ProgressGreen = data["UI"]["progressGreen"];
            ProgressBlue = data["UI"]["progressBlue"];
            ProgressShadowXOffset = data["UI"]["progressShadowXOffset"];
            ProgressShadowYOffset = data["UI"]["progressShadowYOffset"];
            ProgressShadowRed = data["UI"]["progressShadowRed"];
            ProgressShadowGreen = data["UI"]["progressShadowGreen"];
            ProgressShadowBlue = data["UI"]["progressShadowBlue"];
            CompletedExhibitAttenuation = data["UI"]["completedExhibitAttenuation"];
            MsStartingCash = data["UI"]["MSStartingCash"];
            MsCashIncrement = data["UI"]["MSCashIncrement"];
            MsMinCash = data["UI"]["MSMinCash"];
            MsMaxCash = data["UI"]["MSMaxCash"];
            string StartedFirstTutorialStr = data["UI"]["startedFirstTutorial"];
            string StartedDinoTutorialStr = data["UI"]["startedDinoTutorial"];
            string StartedAquaTutorialStr = data["UI"]["startedAquaTutorial"];
            StartedFirstTutorial = Convert.ToBoolean(int.Parse(StartedFirstTutorialStr));
            StartedDinoTutorial = Convert.ToBoolean(int.Parse(StartedDinoTutorialStr));
            StartedAquaTutorial = Convert.ToBoolean(int.Parse(StartedAquaTutorialStr));
            LastWindowX = data["UI"]["lastWindowX"];
            LastWindowY = data["UI"]["lastWindowY"];
            ProgressCalls = data["UI"]["progressCalls"];


        }
    }
}
