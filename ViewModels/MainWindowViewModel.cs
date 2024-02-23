/*
    MainWindowViewModel.cs
    ----------------------
    Defines the MainWindowViewModel class, which is the view model for the main window.
*/

using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using System;
using System.IO;
using System.Linq;

namespace PandaLdr.ViewModels
{
    public class MainWindowViewModel : ViewModelBase
    {
#pragma warning disable CA1822 // Mark members as static
        public string Greeting => "Welcome to Avalonia!";

        static string docPath = Path.GetFullPath("C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\dlupdate");

        public ObservableCollection<string> MyList { get; set; } = new ObservableCollection<string> { };

        //public ObservableCollection<string> MyList
        //{
        //    get => _myList;
        //    set
        //    {
        //        _myList = value;
        //    }
        //}

        public MainWindowViewModel()
        {
            var mods = from mod in Directory.EnumerateFiles(docPath, "*.ztd", SearchOption.TopDirectoryOnly)
                       select new
                       {
                           File = Path.GetFileNameWithoutExtension(mod)
                       };

            foreach (var mod in mods)
            {
                MyList.Add(mod.File);
            }
            
        }


    }
}