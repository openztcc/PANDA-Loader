using Avalonia.Controls;
using System.Diagnostics;
using Avalonia.Interactivity;
using System;
using PandaLdr.Models;

namespace PandaLdr.Views
{
    public partial class HomeView : UserControl
    {
        private string _modPath = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon 2\\dlupdate";
        public HomeView()
        {
            InitializeComponent();
        }
        private void LaunchGame(object? sender, RoutedEventArgs e)
        {
            string path = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\zoo.exe";
            
            if (System.IO.File.Exists(path))
            {
                try
                {
                    Process.Start(new ProcessStartInfo
                    {
                        FileName = path,
                        UseShellExecute = true
                    });
                }
                catch (System.ComponentModel.Win32Exception exc)
                {
                    Console.WriteLine("Error: " + exc.Message);
                }
            }
            else
            {
                Console.WriteLine("zoo.exe not found at {path}");
            }
        }

    }
}