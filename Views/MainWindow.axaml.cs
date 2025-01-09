using Avalonia.Controls;
using Avalonia.Interactivity;
using System;
using System.Diagnostics;
using PandaLdr.Services;
using PandaLdr.ViewModels;

namespace PandaLdr.Views
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            // _settingsService = new IniService("C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\zoo.ini");
            DataContext = new MainWindowViewModel(new IniService("C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\zoo.ini"));
        }


        private void ButtonOnClick(object sender, RoutedEventArgs e)
        {
            Debug.WriteLine("Button clicked");
            // TextBlockText = "Button clicked";
        }
    }

}