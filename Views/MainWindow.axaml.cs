using Avalonia.Controls;
using Avalonia.Interactivity;
using System;
using System.Diagnostics;

namespace PandaLdr.Views
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }


        private void ButtonOnClick(object sender, RoutedEventArgs e)
        {
            Debug.WriteLine("Button clicked");
            TextBlockName.Text = "Button clicked";
            // TextBlockText = "Button clicked";
        }
    }

}