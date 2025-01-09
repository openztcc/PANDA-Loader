using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using PandaLdr.Models;
using System;
using IniParser;
using IniParser.Model;
using PandaLdr.Services;
using Avalonia.Svg.Commands;

namespace PandaLdr.ViewModels
{
    public class ZooIniMessage 
    {

    }
    public partial class HomeViewModel : ViewModelBase
    {
        private readonly ISettingsService _settingsService;
        private readonly string _modPath = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\dlupdate";
        private readonly string _mapPath = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\maps";


        [ObservableProperty]
        private int? _installedModCount;
        [ObservableProperty]
        private int? _installedMapCount;
        [ObservableProperty]
        private string? _pandaVersion;
        [ObservableProperty]
        private string? _oztVersion = "0.1.0";

        public HomeViewModel(ISettingsService settingsService)
        {
            // gets the number of installed mods in the dlupdate folder
            InstalledModCount = ArchiveMgr.GetArchives(_modPath).Count;
            Console.WriteLine("Installed mods: " + InstalledModCount);

            // gets the number of installed maps in the maps folder
            InstalledMapCount = MapMgr.GetMaps(_mapPath).Count;
            Console.WriteLine("Installed maps: " + InstalledMapCount);

            // panda version
            PandaVersion = "0.1.0";

            _settingsService = settingsService;
            LoadIni();
        }


        //------------------------- INI Configuration

        // Display Settings
        [ObservableProperty]
        private bool _fullScreen;
        [ObservableProperty]
        private int _resolution;
        [ObservableProperty]
        private int _updateRate;
        [ObservableProperty]
        private int _drawRate;

        // Debug Settings
        [ObservableProperty]
        private string? _logCutoff;
        [ObservableProperty]
        private bool? _sendLogfile;
        [ObservableProperty]
        private bool? _sendDebugger;
        [ObservableProperty]
        private string? _deltaLog1;
        [ObservableProperty]
        private string? _deltaLog0;
        [ObservableProperty]
        private bool? _drawFps;
        [ObservableProperty]
        private string? _drawFpsX;
        [ObservableProperty]
        private string? _drawFpsY;

        // Manager Settings Section
        [ObservableProperty]
        private string? _aiMgr;
        [ObservableProperty]
        private string? _ztWorldMgr;
        [ObservableProperty]
        private string? _bfGameMgr;
        [ObservableProperty]
        private string? _ztScenarioMgr;
        [ObservableProperty]
        private string? _bfScriptMgr;
        [ObservableProperty]
        private string? _bfSoundMgr;
        [ObservableProperty]
        private string? _ztAdvTerrainMgr;

        // Language Settings Section
        [ObservableProperty]
        private string? _lang;
        [ObservableProperty]
        private string? _sublang;

        // Dependency Settings Section
        [ObservableProperty]
        private string? _res;
        [ObservableProperty]
        private string? _langDep;

        // Resource Settings Section
        [ObservableProperty]
        private string? _path;

        // User Settings Section
        [ObservableProperty]
        private string? _lastFile;
        [ObservableProperty]
        private string? _unlockEntity0;
        [ObservableProperty]
        private string? _unlockCount;
        [ObservableProperty]
        private string? _unlockEntity1;
        [ObservableProperty]
        private string? _unlockEntity2;
        [ObservableProperty]
        private bool? _showUserEntityWarning;

        // Advanced Settings Section
        [ObservableProperty]
        private string? _drag;
        [ObservableProperty]
        private string? _click;
        [ObservableProperty]
        private string? _normal;
        [ObservableProperty]
        private string? _level;
        [ObservableProperty]
        private bool? _loadHalfAnimations;
        [ObservableProperty]
        private bool? _use8BitSound;
        [ObservableProperty]
        private bool? _developerModeEnabled;

        // Map Settings Section
        [ObservableProperty]
        private string? _mapX;
        [ObservableProperty]
        private string? _mapY;

        // UI Settings Section
        [ObservableProperty]
        private bool? _useAlternateCursors;
        [ObservableProperty]
        private string? _tooltipDelay;
        [ObservableProperty]
        private string? _tooltipDuration;
        [ObservableProperty]
        private string? _messageDisplay;
        [ObservableProperty]
        private string? _mouseScrollThreshold;
        [ObservableProperty]
        private string? _mouseScrollDelay;
        [ObservableProperty]
        private string? _mouseScrollX;
        [ObservableProperty]
        private string? _mouseScrollY;
        [ObservableProperty]
        private string? _keyScrollX;
        [ObservableProperty]
        private string? _keyScrollY;
        [ObservableProperty]
        private string? _minimumMessageInterval;
        [ObservableProperty]
        private string? _defaultEditCharLimit;
        [ObservableProperty]
        private bool? _noMenuMusic;  
        [ObservableProperty]
        private string? _menuMusic;
        [ObservableProperty]
        private string? _helpType;
        [ObservableProperty]
        private bool? _playMovie;
        [ObservableProperty]
        private string? _movieVolume1;
        [ObservableProperty]
        private bool? _playSecondMovie;
        [ObservableProperty]
        private string? _movieVolume2;
        [ObservableProperty]
        private string? _maxShortTooltipWidth;
        [ObservableProperty]
        private string? _maxLongTooltipWidth;
        [ObservableProperty]
        private string? _progressLeft;
        [ObservableProperty]
        private string? _progressTop;
        [ObservableProperty]
        private string? _progressRight;
        [ObservableProperty]
        private string? _progressBottom;
        [ObservableProperty]
        private string? _progressRed;
        [ObservableProperty]
        private string? _progressGreen;
        [ObservableProperty]
        private string? _progressBlue;
        [ObservableProperty]
        private string? _progressShadowXOffset;
        [ObservableProperty]
        private string? _progressShadowYOffset;
        [ObservableProperty]
        private string? _progressShadowRed;
        [ObservableProperty]
        private string? _progressShadowGreen;
        [ObservableProperty]
        private string? _progressShadowBlue;
        [ObservableProperty]
        private string? _completedExhibitAttenuation;
        [ObservableProperty]
        private string? _msStartingCash;
        [ObservableProperty]
        private string? _msCashIncrement;
        [ObservableProperty]
        private string? _msMinCash;
        [ObservableProperty]
        private string? _msMaxCash;
        [ObservableProperty]
        private bool? _startedFirstTutorial;
        [ObservableProperty]
        private bool? _startedDinoTutorial;
        [ObservableProperty]
        private bool? _startedAquaTutorial;
        [ObservableProperty]
        private string? _lastWindowX;
        [ObservableProperty]
        private string? _lastWindowY;
        [ObservableProperty]
        private string? _progressCalls;

        // Scenario Settings Section
        [ObservableProperty]
        private string? _tutorial;
        [ObservableProperty]
        private string? _ba;
        [ObservableProperty]
        private string? _cg;
        [ObservableProperty]
        private string? _gc;
        [ObservableProperty]
        private string? _cn;
        [ObservableProperty]
        private string? _bb;


        public RelayCommand SaveCommand => new RelayCommand(SaveIni);

        private void LoadIni()
        {
            // Load INI Configuration

            IniData data = _settingsService.LoadIni();

            // Display Settings
            string FullScreenStr = data["user"]["fullscreen"];
            FullScreen = Convert.ToBoolean(int.Parse(FullScreenStr));
            var screenWidth = data["user"]["screenwidth"];
            var screenHeight = data["user"]["screenheight"];
            var selectedResolution = 0;
            if (screenWidth == "800" && screenHeight == "600")
            {
                selectedResolution = 0;
            }
            else if (screenWidth == "1024" && screenHeight == "768")
            {
                selectedResolution = 1;
            }
            else if (screenWidth == "1280" && screenHeight == "1024")
            {
                selectedResolution = 2;
            }
            else if (screenWidth == "1600" && screenHeight == "1200")
            {
                selectedResolution = 3;
            }
            else if (screenWidth == "1920" && screenHeight == "1080")
            {
                selectedResolution = 4;
            }
            else if (screenWidth == "2560" && screenHeight == "1440")
            {
                selectedResolution = 5;
            }
            else if (screenWidth == "3840" && screenHeight == "2160")
            {
                selectedResolution = 6;
            }
            Resolution = selectedResolution;
            string UpdateRateStr = data["user"]["UpdateRate"];
            switch (UpdateRateStr)
            {
                case "15":
                    UpdateRate = 3;
                    break;
                case "20":
                    UpdateRate = 2;
                    break;
                case "30":
                    UpdateRate = 1;
                    break;  
                case "60":
                    UpdateRate = 0;
                    break;
                default:
                    UpdateRate = 1;
                    break;
            }
            string DrawRateStr = data["user"]["DrawRate"];
            switch (DrawRateStr)
            {
                case "15":
                    DrawRate = 3;
                    break;
                case "20":
                    DrawRate = 2;
                    break;
                case "30":
                    DrawRate = 1;
                    break;
                case "60":
                    DrawRate = 0;
                    break;
                default:
                    DrawRate = 1;
                    break;
            }
            LastFile = data["user"]["lastfile"];
            UnlockEntity0 = data["user"]["unlockEntity0"];
            UnlockCount = data["user"]["unlockCount"];
            UnlockEntity1 = data["user"]["unlockEntity1"];
            UnlockEntity2 = data["user"]["unlockEntity2"];
            string ShowUserEntityWarningStr = data["user"]["showUserEntityWarning"];
            if (ShowUserEntityWarningStr != null)
            {
                ShowUserEntityWarning = Convert.ToBoolean(int.Parse(ShowUserEntityWarningStr));
            }

            // Debug Settings
            LogCutoff = data["debug"]["logCutoff"];
            string SendLogfileStr = data["debug"]["sendLogfile"];
            SendLogfile = Convert.ToBoolean(int.Parse(SendLogfileStr));
            string SendDebuggerStr = data["debug"]["sendDebugger"];
            SendDebugger = Convert.ToBoolean(int.Parse(SendDebuggerStr));
            DeltaLog1 = data["debug"]["deltaLog1"];
            DeltaLog0 = data["debug"]["deltaLog0"];
            string DrawFpsStr = data["debug"]["drawfps"];
            DrawFps = Convert.ToBoolean(int.Parse(DrawFpsStr));
            DrawFpsX = data["debug"]["drawfpsx"];
            DrawFpsY = data["debug"]["drawfpsy"];

            // Manager Settings
            AiMgr = data["mgr"]["aimgr"];
            ZtWorldMgr = data["mgr"]["worldmgr"];
            BfGameMgr = data["mgr"]["gamemgr"];
            ZtScenarioMgr = data["mgr"]["scenariomgr"];
            BfScriptMgr = data["mgr"]["scriptmgr"];
            BfSoundMgr = data["mgr"]["soundmgr"];
            ZtAdvTerrainMgr = data["mgr"]["terrainmgr"];

            // Language Settings
            Lang = data["language"]["lang"];
            Sublang = data["language"]["sublang"];

            // Dependency Settings
            Res = data["lib"]["res"];
            LangDep = data["lib"]["lang"];

            // Resource Settings
            Path = data["resource"]["path"];

            // Advanced Settings
            Drag = data["advanced"]["drag"];
            Click = data["advanced"]["click"];
            Normal = data["advanced"]["normal"];
            Level = data["advanced"]["level"];
            string LoadHalfAnimationsStr = data["advanced"]["loadHalfAnims"];
            if (LoadHalfAnimationsStr != null)
            {
                LoadHalfAnimations = Convert.ToBoolean(int.Parse(LoadHalfAnimationsStr));
            }
            string Use8BitSoundStr = data["advanced"]["use8BitSound"];
            if (Use8BitSoundStr != null)
            {
                Use8BitSound = Convert.ToBoolean(int.Parse(Use8BitSoundStr));
            }
            string DeveloperModeEnabledStr = data["advanced"]["devModeEnabled"];
            if (DeveloperModeEnabledStr != null)
            {
                DeveloperModeEnabled = Convert.ToBoolean(int.Parse(DeveloperModeEnabledStr));
            }

            // Map Settings
            MapX = data["Map"]["mapX"];
            MapY = data["Map"]["mapY"];

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
            if (StartedDinoTutorialStr != null)
            {
                StartedDinoTutorial = Convert.ToBoolean(int.Parse(StartedDinoTutorialStr));
            }
            if (StartedAquaTutorialStr != null)
            {
                StartedAquaTutorial = Convert.ToBoolean(int.Parse(StartedAquaTutorialStr));
            }
            LastWindowX = data["UI"]["lastWindowX"];
            LastWindowY = data["UI"]["lastWindowY"];
            ProgressCalls = data["UI"]["progresscalls"];

            // Scenario Settings
            Tutorial = data["scenario"]["tutorial"];
            Ba = data["scenario"]["ba"];
            Cg = data["scenario"]["cg"];
            Gc = data["scenario"]["gc"];
            Cn = data["scenario"]["cn"];
            Bb = data["scenario"]["bb"];
        }

        private void SaveIni()
        {
            IniData data = new IniData();

            // Display Settings
            data["user"]["fullscreen"] = Convert.ToInt32(FullScreen).ToString();
            switch (Resolution)
            {
                case 0:
                    data["user"]["screenwidth"] = "800";
                    data["user"]["screenheight"] = "600";
                    break;
                case 1:
                    data["user"]["screenwidth"] = "1024";
                    data["user"]["screenheight"] = "768";
                    break;
                case 2:
                    data["user"]["screenwidth"] = "1280";
                    data["user"]["screenheight"] = "1024";
                    break;
                case 3:
                    data["user"]["screenwidth"] = "1600";
                    data["user"]["screenheight"] = "1200";
                    break;
                case 4:
                    data["user"]["screenwidth"] = "1920";
                    data["user"]["screenheight"] = "1080";
                    break;
                case 5:
                    data["user"]["screenwidth"] = "2560";
                    data["user"]["screenheight"] = "1440";
                    break;
                case 6:
                    data["user"]["screenwidth"] = "3840";
                    data["user"]["screenheight"] = "2160";
                    break;
                default:
                    data["user"]["screenwidth"] = "1024";
                    data["user"]["screenheight"] = "768";
                    break;
            }
            switch (UpdateRate)
            {
                case 0:
                    data["user"]["UpdateRate"] = "60";
                    break;
                case 1:
                    data["user"]["UpdateRate"] = "30";
                    break;
                case 2:
                    data["user"]["UpdateRate"] = "20";
                    break;
                case 3:
                    data["user"]["UpdateRate"] = "15";
                    break;
                default:
                    data["user"]["UpdateRate"] = "30";
                    break;
            }
            switch (DrawRate)
            {
                case 0:
                    data["user"]["DrawRate"] = "60";
                    break;
                case 1:
                    data["user"]["DrawRate"] = "30";
                    break;
                case 2:
                    data["user"]["DrawRate"] = "20";
                    break;
                case 3:
                    data["user"]["DrawRate"] = "15";
                    break;
                default:
                    data["user"]["DrawRate"] = "30";
                    break;
            }
            data["user"]["lastfile"] = LastFile;
            data["user"]["unlockEntity0"] = UnlockEntity0;
            data["user"]["unlockCount"] = UnlockCount;
            data["user"]["unlockEntity1"] = UnlockEntity1;
            data["user"]["unlockEntity2"] = UnlockEntity2;
            data["user"]["showUserEntityWarning"] = Convert.ToInt32(ShowUserEntityWarning).ToString();

            // Debug Settings
            data["debug"]["logCutoff"] = LogCutoff;
            data["debug"]["sendLogfile"] = Convert.ToInt32(SendLogfile).ToString();
            data["debug"]["sendDebugger"] = Convert.ToInt32(SendDebugger).ToString();
            data["debug"]["deltaLog1"] = DeltaLog1;
            data["debug"]["deltaLog0"] = DeltaLog0;
            data["debug"]["drawfps"] = Convert.ToInt32(DrawFps).ToString();
            data["debug"]["drawfpsx"] = DrawFpsX;
            data["debug"]["drawfpsy"] = DrawFpsY;

            // Manager Settings
            data["mgr"]["aimgr"] = AiMgr;
            data["mgr"]["worldmgr"] = ZtWorldMgr;
            data["mgr"]["gamemgr"] = BfGameMgr;
            data["mgr"]["scenariomgr"] = ZtScenarioMgr;
            data["mgr"]["scriptmgr"] = BfScriptMgr;
            data["mgr"]["soundmgr"] = BfSoundMgr;
            data["mgr"]["terrainmgr"] = ZtAdvTerrainMgr;

            // Language Settings
            data["language"]["lang"] = Lang;
            data["language"]["sublang"] = Sublang;

            // Dependency Settings
            data["lib"]["res"] = Res;
            data["lib"]["lang"] = LangDep;

            // Resource Settings
            data["resource"]["path"] = Path;

            // Advanced Settings
            data["advanced"]["drag"] = Drag;
            data["advanced"]["click"] = Click;
            data["advanced"]["normal"] = Normal;
            data["advanced"]["level"] = Level;
            data["advanced"]["loadHalfAnims"] = Convert.ToInt32(LoadHalfAnimations).ToString();
            data["advanced"]["use8BitSound"] = Convert.ToInt32(Use8BitSound).ToString();
            data["advanced"]["devModeEnabled"] = Convert.ToInt32(DeveloperModeEnabled).ToString();

            // Map Settings
            data["Map"]["mapX"] = MapX;
            data["Map"]["mapY"] = MapY;

            // UI Settings
            data["UI"]["useAlternateCursors"] = Convert.ToInt32(UseAlternateCursors).ToString();
            data["UI"]["tooltipDelay"] = TooltipDelay;
            data["UI"]["tooltipDuration"] = TooltipDuration;
            data["UI"]["MessageDisplay"] = MessageDisplay;
            data["UI"]["mouseScrollThreshold"] = MouseScrollThreshold;
            data["UI"]["mouseScrollDelay"] = MouseScrollDelay;
            data["UI"]["mouseScrollX"] = MouseScrollX;
            data["UI"]["mouseScrollY"] = MouseScrollY;
            data["UI"]["keyScrollX"] = KeyScrollX;
            data["UI"]["keyScrollY"] = KeyScrollY;
            data["UI"]["minimumMessageInterval"] = MinimumMessageInterval;
            data["UI"]["defaultEditCharLimit"] = DefaultEditCharLimit;
            data["UI"]["noMenuMusic"] = Convert.ToInt32(NoMenuMusic).ToString();
            data["UI"]["menuMusic"] = MenuMusic;
            data["UI"]["helpType"] = HelpType;
            data["UI"]["playMovie"] = Convert.ToInt32(PlayMovie).ToString();
            data["UI"]["movievolume1"] = MovieVolume1;
            data["UI"]["playSecondMovie"] = Convert.ToInt32(PlaySecondMovie).ToString();
            data["UI"]["movievolume2"] = MovieVolume2;
            data["UI"]["maxShortTooltipWidth"] = MaxShortTooltipWidth;
            data["UI"]["maxLongTooltipWidth"] = MaxLongTooltipWidth;
            data["UI"]["progressLeft"] = ProgressLeft;
            data["UI"]["progressTop"] = ProgressTop;
            data["UI"]["progressRight"] = ProgressRight;
            data["UI"]["progressBottom"] = ProgressBottom;
            data["UI"]["progressRed"] = ProgressRed;
            data["UI"]["progressGreen"] = ProgressGreen;
            data["UI"]["progressBlue"] = ProgressBlue;
            data["UI"]["progressShadowXOffset"] = ProgressShadowXOffset;
            data["UI"]["progressShadowYOffset"] = ProgressShadowYOffset;
            data["UI"]["progressShadowRed"] = ProgressShadowRed;
            data["UI"]["progressShadowGreen"] = ProgressShadowGreen;
            data["UI"]["progressShadowBlue"] = ProgressShadowBlue;
            data["UI"]["completedExhibitAttenuation"] = CompletedExhibitAttenuation;
            data["UI"]["MSStartingCash"] = MsStartingCash;
            data["UI"]["MSCashIncrement"] = MsCashIncrement;
            data["UI"]["MSMinCash"] = MsMinCash;
            data["UI"]["MSMaxCash"] = MsMaxCash;
            data["UI"]["startedFirstTutorial"] = Convert.ToInt32(StartedFirstTutorial).ToString();
            data["UI"]["startedDinoTutorial"] = Convert.ToInt32(StartedDinoTutorial).ToString();
            data["UI"]["startedAquaTutorial"] = Convert.ToInt32(StartedAquaTutorial).ToString();
            data["UI"]["lastWindowX"] = LastWindowX;
            data["UI"]["lastWindowY"] = LastWindowY;
            data["UI"]["progresscalls"] = ProgressCalls;
        
            // Scenario Settings
            data["scenario"]["tutorial"] = Tutorial;
            data["scenario"]["ba"] = Ba;
            data["scenario"]["cg"] = Cg;
            data["scenario"]["gc"] = Gc;
            data["scenario"]["cn"] = Cn;
            data["scenario"]["bb"] = Bb;

            _settingsService.SaveIni(data);
        }
    }
}
