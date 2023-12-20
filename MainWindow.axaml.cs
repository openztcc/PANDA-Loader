using Avalonia.Controls;
using Avalonia.Markup.Xaml;
using System.Collections.Generic;

namespace PandaLdr
{
    public partial class MainWindow : Window
    {
        public List<string> MyList { get; set; }

        public MainWindow()
        {
            InitializeComponent();
            MyList = new List<string>();
            PopulateList();
        }

        private void PopulateList()
        {
            MyList.Add("Item 1");
            MyList.Add("Item 2");
            MyList.Add("Item 3");
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