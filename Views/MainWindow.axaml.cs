using Avalonia.Controls;
using Avalonia.Markup.Xaml;
using System.Collections.Generic;
using System.Collections.ObjectModel;

namespace PandaLdr.Views
{
    public partial class MainWindow : Window
    {
        public ObservableCollection<string> MyList { get; set; } = new ObservableCollection<string>(new string[] { "Row 1", "Row 2", "Row 3" });

        public MainWindow()
        {
            InitializeComponent();
            DataContext = this;
        }

        private void InitializeComponent()
        {
            AvaloniaXamlLoader.Load(this);
        }

        private void Button_Click(object sender, Avalonia.Interactivity.RoutedEventArgs e)
        {

        }
    }
}