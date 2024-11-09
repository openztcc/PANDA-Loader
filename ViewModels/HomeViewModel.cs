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

        public HomeViewModel()
        {
            InstalledModCount = ArchiveMgr.GetArchives(_modPath).Count;
            Console.WriteLine("Installed mods: " + InstalledModCount);

            InstalledMapCount = MapMgr.GetMaps(_mapPath).Count;
            Console.WriteLine("Installed maps: " + InstalledMapCount);
        }
    }
}
