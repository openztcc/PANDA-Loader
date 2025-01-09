using Avalonia.Controls;
using System.Diagnostics;
using Avalonia.Interactivity;
using System;
using PandaLdr.Models;

namespace PandaLdr.Views
{
    public partial class HomeView : UserControl
    {
        private string? _path;

        public HomeView()
        {
            InitializeComponent();
        }
        
        public HomeView(string path)
        {
            InitializeComponent();
            _path = path;
        }
        private void LaunchGame(object? sender, RoutedEventArgs e)
        {
            if (System.IO.File.Exists(_path))
            {
                try
                {
                    Process.Start(new ProcessStartInfo
                    {
                        FileName = _path,
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
                Console.WriteLine("zoo.exe not found at {_path}");
            }
        }

    }
}